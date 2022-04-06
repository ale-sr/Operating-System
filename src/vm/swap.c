#include "vm/swap.h"
#include <bitmap.h>
#include <debug.h>
#include <stdio.h>
#include "vm/frame.h"
#include "vm/page.h"
#include "threads/synch.h"
#include "threads/vaddr.h"
#include "devices/block.h"

static struct block* global_swap_block;
static struct bitmap* global_bitmap;

#define SWAP_SIZE 1024

static struct lock sl;


/* Sets up swap. */
void swap_init (void) 
{
  global_swap_block = block_get_role(BLOCK_SWAP);
  if(global_swap_block==NULL){
    PANIC("No se puede inicializar el swap");
  }

  global_bitmap = bitmap_create (block_size (global_swap_block) / (PGSIZE / BLOCK_SECTOR_SIZE));
  if(global_bitmap==NULL){
      PANIC("No se puede inicializar el bitmap");
  }

  lock_init(&sl);


}

/* Swaps in page P, which must have a locked frame
   (and be swapped out). */

void swap_in (struct page *p) 
{

  ASSERT (p->frame != NULL);
  ASSERT (lock_held_by_current_thread (&p->frame->lock));
  ASSERT (p->sector != (block_sector_t) -1);



  lock_acquire(&sl);
  for(int i = 0; i < (PGSIZE/BLOCK_SECTOR_SIZE);i++){
    block_read(global_swap_block , i + p->sector, (i * BLOCK_SECTOR_SIZE) + p->frame->base);
  }
  

  bitmap_reset(global_bitmap, p->sector/ (PGSIZE/BLOCK_SECTOR_SIZE));
  lock_release(&sl);



  p->sector = (block_sector_t) -1;
}


/* Swaps out page P, which must have a locked frame. */
bool swap_out (struct page *p) 
{
  ASSERT (p->frame != NULL);
  ASSERT (lock_held_by_current_thread (&p->frame->lock));
  
  
  lock_acquire(&sl);
  size_t idx = bitmap_scan_and_flip(global_bitmap, 0,1,0);
  lock_release(&sl);


  if(idx == BITMAP_ERROR )
    return false;


  p->sector = (PGSIZE / BLOCK_SECTOR_SIZE) * idx;

  for(int i = 0; i < (PGSIZE / BLOCK_SECTOR_SIZE);i++){
    block_write(global_swap_block , i + p->sector, (i * BLOCK_SECTOR_SIZE) + p->frame->base );
  }


  p->file = NULL;
  p->file_offset = 0;
  p->file_bytes = 0;
  p->private = false;

  return true;
  
}
