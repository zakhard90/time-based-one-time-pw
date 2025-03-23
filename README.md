# Time-based one-time password
A simple CLI implementation of a time-based one-time password generator (TOTP) in C.

## Instructions

Works on Ubuntu and WSL2 with Ubuntu. You might have to adjust the Makefile if you have a different OS.

First install gcc, make, openssl and libssl:

`sudo apt update`

`sudo apt install gcc make openssl liibssl-dev`

Then create a `.env` file and add your base32 secret code as shown in the `.env.sample`.

Compile the project by running the `make` command in your terminal.

Run the program `./bin/totp`.
You should see an output like this:

```
Your OTP is: 123456
```

In case you want to test the accuracy of the output, you can use the same base32 code in any TOTP app (i.e. Google's Authenticator, Open Authenticator, etc.).