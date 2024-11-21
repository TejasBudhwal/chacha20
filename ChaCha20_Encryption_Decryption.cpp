#include <iostream>
#include <vector>
#include <cstdint>
#include <cstring> // For memcpy
#include <iomanip> // For hex output

using namespace std;

// Constants for ChaCha20
const uint32_t CONSTANTS[4] = {0x61707865, 0x3320646E, 0x79622D32, 0x6B206574};

// Rotate left
inline uint32_t ROTL(uint32_t x, uint32_t n) 
{
    return (x << n) | (x >> (32 - n));
}

// Quarter-round function
void quarterRound(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d) 
{
    a += b; d ^= a; d = ROTL(d, 16);
    c += d; b ^= c; b = ROTL(b, 12);
    a += b; d ^= a; d = ROTL(d, 8);
    c += d; b ^= c; b = ROTL(b, 7);
}

// ChaCha20 block function (full 20 rounds)
void chacha20Block(uint32_t state[16], uint32_t output[16]) 
{
    memcpy(output, state, 16 * sizeof(uint32_t)); // Copy the input state to working state

    // Perform 20 rounds (10 column + 10 diagonal)
    for (int i = 0; i < 10; ++i) {
        // Column rounds
        quarterRound(output[0], output[4], output[8], output[12]);
        quarterRound(output[1], output[5], output[9], output[13]);
        quarterRound(output[2], output[6], output[10], output[14]);
        quarterRound(output[3], output[7], output[11], output[15]);

        // Diagonal rounds
        quarterRound(output[0], output[5], output[10], output[15]);
        quarterRound(output[1], output[6], output[11], output[12]);
        quarterRound(output[2], output[7], output[8], output[13]);
        quarterRound(output[3], output[4], output[9], output[14]);
    }

    // Add the original state to the output state
    for(int i=0; i<16; ++i) 
    {
        output[i]+=state[i];
    }
}

// Print the state for debugging
void printState(const char *label, const uint32_t state[16]) 
{
    cout << label << ": ";
    for (int i = 0; i < 16; ++i) {
        cout << hex << setw(8) << setfill('0') << state[i] << " ";
    }
    cout << endl;
}

// ChaCha20 encryption and decryption
void chacha20EncryptDecrypt(const vector<uint8_t> &key, const vector<uint8_t> &nonce, uint32_t counter, const vector<uint8_t> &input, vector<uint8_t> &output) 
{
    // Verify key and nonce sizes
    if (key.size() != 32 || nonce.size() != 12) 
    {
        throw invalid_argument("Key must be 32 bytes and nonce must be 12 bytes.");
    }

    // Initialize the state
    uint32_t state[16] = 
    {
        CONSTANTS[0], CONSTANTS[1], CONSTANTS[2], CONSTANTS[3],           // Constants
        // Key (8 words)
        uint32_t(key[0]) | (uint32_t(key[1]) << 8) | (uint32_t(key[2]) << 16) | (uint32_t(key[3]) << 24),
        uint32_t(key[4]) | (uint32_t(key[5]) << 8) | (uint32_t(key[6]) << 16) | (uint32_t(key[7]) << 24),
        uint32_t(key[8]) | (uint32_t(key[9]) << 8) | (uint32_t(key[10]) << 16) | (uint32_t(key[11]) << 24),
        uint32_t(key[12]) | (uint32_t(key[13]) << 8) | (uint32_t(key[14]) << 16) | (uint32_t(key[15]) << 24),
        uint32_t(key[16]) | (uint32_t(key[17]) << 8) | (uint32_t(key[18]) << 16) | (uint32_t(key[19]) << 24),
        uint32_t(key[20]) | (uint32_t(key[21]) << 8) | (uint32_t(key[22]) << 16) | (uint32_t(key[23]) << 24),
        uint32_t(key[24]) | (uint32_t(key[25]) << 8) | (uint32_t(key[26]) << 16) | (uint32_t(key[27]) << 24),
        uint32_t(key[28]) | (uint32_t(key[29]) << 8) | (uint32_t(key[30]) << 16) | (uint32_t(key[31]) << 24),
        counter,                                                       // Counter
        uint32_t(nonce[0]) | (uint32_t(nonce[1]) << 8) | (uint32_t(nonce[2]) << 16) | (uint32_t(nonce[3]) << 24),
        uint32_t(nonce[4]) | (uint32_t(nonce[5]) << 8) | (uint32_t(nonce[6]) << 16) | (uint32_t(nonce[7]) << 24),
        uint32_t(nonce[8]) | (uint32_t(nonce[9]) << 8) | (uint32_t(nonce[10]) << 16) | (uint32_t(nonce[11]) << 24)
    };

    printState("Initial state", state);

    // Encryption/decryption
    output.resize(input.size());
    size_t blockCount = (input.size() + 63) / 64; // Calculate number of 64-byte blocks
    uint8_t keystream[64];

    for (size_t block = 0; block < blockCount; ++block) 
    {
        uint32_t outputState[16];
        chacha20Block(state, outputState); // Generate keystream block

        printState("Keystream block state", outputState);

        // Convert keystream to bytes
        for (int i = 0; i < 16; ++i) {
            keystream[i * 4 + 0] = (outputState[i] >> 0) & 0xFF;
            keystream[i * 4 + 1] = (outputState[i] >> 8) & 0xFF;
            keystream[i * 4 + 2] = (outputState[i] >> 16) & 0xFF;
            keystream[i * 4 + 3] = (outputState[i] >> 24) & 0xFF;
        }

        cout << "Keystream (hex): ";
        for(int i=0; i<64; ++i) 
        {
            cout<<hex<<setw(2)<<setfill('0')<<int(keystream[i])<<" ";
        }
        cout<<endl;

        // XOR input with keystream
        for(size_t i=0; i<64 && (block*64+i)<input.size(); ++i) 
        {
            output[block*64+i]=input[block*64+i]^keystream[i];
        }

        // Increment counter
        state[12]++;
    }
}

// Example usage
int main() {
    vector<uint8_t> key(32, 0x00);  // 32-byte key (all taken as zeroes for simplicity)
    vector<uint8_t> nonce(12, 0x00); // 12-byte nonce (all taken as zeroes for simplicity)
    uint32_t counter=1;           // Starting counter
    vector<uint8_t> plaintext = { 'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!' }; // Example plaintext
    vector<uint8_t> ciphertext, decrypted;

    // Encrypt
    cout<<"\nENCRYPTION PROCESS\n";
    chacha20EncryptDecrypt(key, nonce, counter, plaintext, ciphertext);

    // Display ciphertext
    cout<<"\nCiphertext (hex): ";
    for(uint8_t byte : ciphertext) cout<<hex<<setw(2)<<setfill('0')<<int(byte)<<" ";
    cout<<endl;

    // Decrypt
    cout<<"\nDECRYPTION PROCESS\n";
    chacha20EncryptDecrypt(key, nonce, counter, ciphertext, decrypted);

    // Display decrypted text
    cout<<"\nDecrypted text: ";
    for(char c : decrypted) cout<<c;
    cout<<endl;

    return 0;
}