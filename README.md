# ChaCha20 Encryption, Decryption, and Differential Cryptanalysis

This repository contains the implementation of the ChaCha20 stream cipher, with two key components:

1. **Chacha20_Encryption_Decryption**: This code demonstrates the encryption and decryption process of the ChaCha20 stream cipher.
2. **Chacha20_Differential**: This code performs a simulation of differential cryptanalysis on a toy example of ChaCha20 to observe the avalanche effect and the cipher's resistance to differential attacks.

## Chacha20_Encryption_Decryption

### Overview

This section provides the code for the encryption and decryption processes of the **ChaCha20** stream cipher. The ChaCha20 cipher is a modern stream cipher designed by Daniel J. Bernstein, known for its efficiency and resistance to cryptanalytic attacks.

The implementation demonstrates the following:
- Key and nonce initialization.
- Quarter-round function.
- 20 rounds of ChaCha20 transformations.
- Keystream generation and XOR operation for encryption and decryption.

### Example

In this example, the program will take a 256-bit key, a 96-bit nonce, and some plaintext to encrypt and decrypt.

Key: 0x0000000000000000000000000000000000000000000000000000000000000000
Nonce: 0x000000000000000000000000
Plaintext: Hello, ChaCha20!
Encrypted Text: 65c1a52d601d084a2c3fd8be

## Chacha20_Differential

### Overview

This section contains a code that performs differential cryptanalysis on the ChaCha20 cipher using a toy example. The simulation demonstrates how differential cryptanalysis exploits the avalanche effect in the cipher to detect weaknesses.

The code:
- Performs differential attacks on a simplified ChaCha20-like structure.
- Observes key differences in ciphertexts for slight variations in inputs.
- Analyzes the results to assess the resistance of the cipher.

### Example

In this example, the code will apply differential cryptanalysis to the ChaCha20-like structure and report findings related to key differences in the outputs based on minimal changes in input.
