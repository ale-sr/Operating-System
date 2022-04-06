CS2S01 Operating systems 
=======================

This repository contains an skeleton for the PintOS Project 3 - Virtual Memory.

## Authors

- Alexandra Shulca Romero
- Jean Paul Huby
- Diego Paredes

## Project Setup

**Requirements:**
- [Docker](https://docs.docker.com/get-docker/)

Don't forget to run the following commands in the same folder as the Dockerfile

- Create the PintOS image

```console
foo@bar:~$ docker build -t pintos-vm-image .
```

- Create a docker volume to made your changes persistent

```console
foo@bar:~$ docker create -it --volume $(pwd)/src:/pintos/src --name <my-volume-name> pintos-vm-image
```

## Run the container

To start working on your container, you need to run:

```console
foo@bar:~$ docker start <my-volume-name>
foo@bar:~$ docker exec -it <my-volume-name> bash
```

To test if you setup is correctly builded, inside the container run the following command and see the following output:

```console
foo@bar:~$ pintos -q run alarm-multiple
...
(alarm-multiple) end
Execution of 'alarm-multiple' complete.
Timer: 587 ticks
Thread: 0 idle ticks, 587 kernel ticks, 0 user ticks
Console: 2954 characters output
Keyboard: 0 keys pressed
Powering off..
```


## References

- [For the dockerfile](https://github.com/JohnStarich/docker-pintos)
- [For the .gitignore](https://github.com/Berkeley-CS162/group0/blob/master/.gitignore)

