
# http server

### About 

Implementation of an [http](https://en.wikipedia.org/wiki/HTTP) server in C++, and further containerization via Docker

### Built With

+ [C++](https://cbrnsolafact.com/)
+ [CMake](https://cmake.org/) 
+ [Docker](https://www.docker.com/)

### Building and Running

Install docker on the machine and configure it.

1. Clone repo to machine

```
git clone https://github.com/ArturRomanchenko/server.git
```

2. Go to the repository folder ```cd server/```

3. Build the docker image

```
docker-compose build
```

4. Start the server

```
docker-compose up
```

### When you are done with the server and need to shut it down

1. Closing the server in the terminal where it is running.
```
Ctrl + C
```

2. Open a second terminal window in the project directory where you cloned the repository

```
docker-compose down
```

### NOTE: if you made a change to the server files, rebuild the docker container to update

### Usage

When you start the docker container ```docker-compose up```, go to your favorite browser and write in the search bar: ```http://localhost:8080/``` and press to ```F5```

### Contact

[Telegram](https://t.me/woshetitelniy)
