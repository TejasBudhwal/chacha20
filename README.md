# ChaCha20 Encryption, Decryption, and Differential Cryptanalysis

This repository contains the implementation of the ChaCha20 stream cipher, with two key components:

1. **Chacha20_Encryption_Decryption**: This code demonstrates the encryption and decryption process of the ChaCha20 stream cipher.
2. **Chacha20_Differential**: This code performs a simulation of differential cryptanalysis on a toy example of ChaCha20 to observe the avalanche effect and the cipher's resistance to differential attacks.

## Table of Contents

- [Chacha20_Encryption_Decryption](#chacha20_encryption_decryption)
  - [Overview](#overview)
  - [Example](#example)
- [Chacha20_Differential](#chacha20_differential)
  - [Overview](#overview-1)
  - [Example](#example-1)
- [License](#license)

## Chacha20_Encryption_Decryption

### Overview

This section provides the code for the encryption and decryption processes of the **ChaCha20** stream cipher. The ChaCha20 cipher is a modern stream cipher designed by Daniel J. Bernstein, known for its efficiency and resistance to cryptanalytic attacks.

The implementation demonstrates the following:
- Key and nonce initialization.
- Quarter-round function.
- 20 rounds of ChaCha20 transformations.
- Keystream generation and XOR operation for encryption and decryption.
