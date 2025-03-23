# Time-Based One-Time Password (TOTP)

A simple CLI implementation of a **Time-Based One-Time Password (TOTP)** generator in C.

## Instructions

This program works on **Ubuntu** and **WSL2 with Ubuntu**. You may need to modify the `Makefile` if you're using a different OS.

### Prerequisites

First, install the required dependencies:

```sh
sudo apt update
sudo apt install gcc make openssl libssl-dev
```

### Setup

1. Create a `.env` file and add your **Base32** secret key as shown in `.env.sample`.
2. Compile the project by running:

   ```sh
   make
   ```
3. Run the program:

   ```sh
   ./bin/totp
   ```

   You should see an output like this:

   ```sh
   Your OTP is: 123456
   ```

### Testing the OTP

To verify the accuracy of the generated OTP, use the same **Base32 secret key** in any **TOTP-compatible** application, such as:
- **Google Authenticator**
- **Open Authenticator**
- **FreeOTP**

## How It Works

TOTP generates a **6-digit** password that remains valid for **30 seconds**. This mechanism is widely used in **Two-Factor Authentication (2FA)** for securing user accounts against unauthorized access.

The algorithm follows the **RFC 6238** standard. It starts with a **Base32-encoded secret key**, which must be securely stored by both the client (user) and server (service provider). If an attacker gains access to the secret key, they can generate valid TOTPs effortlessly.

### Base32 Encoding

Base32 encodes information using **32 alphanumeric characters**. TOTP follows the **RFC 4648** standard, using the characters:

```
A B C D E F G H I J K L M N O P Q R S T U V W X Y Z 2 3 4 5 6 7
```

Given a Base32-encoded secret, the first step is to **decode it into bytes**:

```
Base32:     ORUGS43JONQWI5LNNV4XMYLM
Hex bytes:  74 68 69 73 69 73 61 64 75 6d 6d 79 76 61 6c
```

### Timestamp Calculation

The **Unix epoch timestamp** (current time in seconds) is divided by **30** (the TOTP time step):

```
1742747750 / 30
```

This value is then encoded into an **8-byte array**:

```
00 00 00 00 03 76 68 47
```

### HMAC-SHA1 Hashing

Using the **SHA1** hash algorithm, we compute the HMAC value with the secret key:

```
75 e1 b2 c8 cb d9 94 b2 7b de aa c6 2d c4 dd 4f e6 5d 64 fd
```

### OTP Extraction

To derive the **6-digit OTP**, we:
1. Take the **last 4 bits** of the HMAC value.
2. Use it as an **index** to extract 4 adjacent bytes.
3. Format the result as a 6-digit integer:

```
354598
```

## License

This project is open-source and available under the **MIT License**.