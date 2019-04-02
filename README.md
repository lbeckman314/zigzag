# zigzag

<img src="zigzag.png" width=250/>

A homegrown TCP chat system built with love.

-----

** QUICK START

```sh
python3 zigzag-server.py PORT
make && ./zigzag-client.out ADDRESS PORT
```

where ADDRESS is the IP address of the host running the server, and PORT is the port number that the server is running on.

-----

** EXTRA CREDIT

"Make it possible for either host to send at any time (while the connection is active) without “taking turns”."

-----

** SERVER

To run the server, enter:

```sh
python3 zigzag-server.py PORT
```

where PORT is any open port on the host system.

A shell script — available-port.sh — is made available to the user so they do not have to guess an open port. It scans for open ports on the host system, and returns a random open port. This can be helpful on busy multiuser systems. To use it to run the server, enter:

```sh
chmod u+x available-port.sh
python3 zigzag-server.py `./available-port.sh`
```

Upon successful startup, the server will output the following, including the IP address and port number that may be used to connect a client to the server.

```sh
 ______     __     ______     ______     ______     ______
/\___  \   /\ \   /\  ___\   /\___  \   /\  __ \   /\  ___\
\/_/  /__  \ \ \  \ \ \__ \  \/_/  /__  \ \  __ \  \ \ \__ \
  /\_____\  \ \_\  \ \_____\   /\_____\  \ \_\ \_\  \ \_____\
  \/_____/   \/_/   \/_____/   \/_____/   \/_/\/_/   \/_____/

The server is ready to receive.
IP address: 192.168.169.36
port number: 12000
```

-----

** CLIENT

To run the client, enter:

```sh
make
./zigzag-client.out ADDRESS PORT
```

where ADDRESS is the IP address of the host running the server, and PORT is the port number that the server is running on.

Upon successful startup, the client will output the following, letting the user know that it has successfully connected to the server.


```sh
 _____
|_  (_)__ _ _____ _ __ _
 / /| / _` |_ / _` / _` |
/___|_\__, /__\__,_\__, |
      |___/        |___/

Connected to server.
Enter your nickname:
```

-----

** UNINSTALLATION

To delete the program and all associated files (including this README and the zip file), run:

```sh
make uninstall
```
