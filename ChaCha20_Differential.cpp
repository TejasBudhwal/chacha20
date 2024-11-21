#include <bits/stdc++.h>

using namespace std;

// Constants
const uint32_t CONSTANTS[4] = 
{
    0x61707865, // "expa"
    0x3320646e, // "nd 3"
    0x79622d32, // "2-by"
    0x6b206574  // "te k"
};

// Circular left shift
inline uint32_t ROTL(uint32_t value, int shift) {
    return (value << shift) | (value >> (32 - shift));
}

// Quarter round
void QuarterRound(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d) 
{
    a += b; d ^= a; d = ROTL(d, 16);
    c += d; b ^= c; b = ROTL(b, 12);
    a += b; d ^= a; d = ROTL(d, 8);
    c += d; b ^= c; b = ROTL(b, 7);
}

// ChaCha20 Block Function
void ChaCha20Block(uint32_t state[16], uint32_t output[16]) 
{
    memcpy(output, state, 16*sizeof(uint32_t));

    // 20 rounds (where we have 10 column + 10 diagonal rounds)
    for (int i=0; i<10; ++i) 
    {
        // Column round
        QuarterRound(output[0], output[4], output[8], output[12]);
        QuarterRound(output[1], output[5], output[9], output[13]);
        QuarterRound(output[2], output[6], output[10], output[14]);
        QuarterRound(output[3], output[7], output[11], output[15]);
        // Diagonal round
        QuarterRound(output[0], output[5], output[10], output[15]);
        QuarterRound(output[1], output[6], output[11], output[12]);
        QuarterRound(output[2], output[7], output[8], output[13]);
        QuarterRound(output[3], output[4], output[9], output[14]);
    }

    // Add the original state to the output
    for(int i=0; i<16; ++i) 
    {
        output[i]+=state[i];
    }
}

// Key setup function
void SetupChaCha20State(uint32_t state[16], const uint8_t key[32], const uint8_t nonce[12], uint32_t counter) 
{
    for (int i=0; i<4; ++i) 
    {
        state[i] = CONSTANTS[i];
    }
    // Key
    for (int i=0; i<8; ++i) 
    {
        state[4+i]=((uint32_t)key[i * 4 + 0] << 0) | ((uint32_t)key[i * 4 + 1] << 8) | ((uint32_t)key[i * 4 + 2] << 16) | ((uint32_t)key[i * 4 + 3] << 24);
    }
    // Counter
    state[12]=counter;
    // Nonce
    for(int i=0; i<3; ++i) 
    {
        state[13+i]=((uint32_t)nonce[i * 4 + 0] << 0) | ((uint32_t)nonce[i * 4 + 1] << 8) | ((uint32_t)nonce[i * 4 + 2] << 16) | ((uint32_t)nonce[i * 4 + 3] << 24);
    }
}

// Convert State to Bytes
void StateToBytes(uint32_t output[16], uint8_t stream[64]) 
{
    for(int i=0; i<16; ++i) 
    {
        stream[i*4+0] = (output[i]>>0) & 0xFF;
        stream[i*4+1] = (output[i]>>8) & 0xFF;
        stream[i*4+2] = (output[i]>>16) & 0xFF;
        stream[i*4+3] = (output[i]>>24) & 0xFF;
    }
}

// Print Stream in Hexadecimal
void PrintStream(const uint8_t stream[64]) 
{
    for(int i=0; i<64; ++i) 
    {
        cout<<hex<<setw(2)<<setfill('0')<<(int)stream[i];
        if ((i+1)%4==0) cout<<" ";
        if ((i+1)%16==0) cout<<endl;
    }
}

// Simulate Differential Attack
void SimulateAttack(const uint8_t key[32], const uint8_t nonce1[12], const uint8_t nonce2[12], uint32_t counter) 
{
    uint32_t state1[16], state2[16], output1[16], output2[16];
    uint8_t stream1[64], stream2[64];

    // Set up states
    SetupChaCha20State(state1, key, nonce1, counter);
    SetupChaCha20State(state2, key, nonce2, counter);

    // Generate keystreams
    ChaCha20Block(state1, output1);
    ChaCha20Block(state2, output2);

    StateToBytes(output1, stream1);
    StateToBytes(output2, stream2);

    // Print keystreams
    cout<<"Keystream 1 (Nonce 1):"<<endl;
    PrintStream(stream1);
    cout<<"\nKeystream 2 (Nonce 2):"<<endl;
    PrintStream(stream2);

    // Calculating differences
    int diff_count=0;
    cout<<"\nByte Differences Between Keystreams:"<<endl;
    for(int i=0; i<64; ++i) 
    {
        if(stream1[i]!=stream2[i]) 
        {
            cout<<"Byte "<<i<<": "<<hex<<setw(2)<<(int)stream1[i]<<" -> "<<setw(2)<<(int)stream2[i]<<endl;
            ++diff_count;
        }
    }
    cout<<"\nTotal differing bytes: "<<dec<<diff_count<<endl;
}

int main() 
{
    // Example Key (32 bytes) and Nonce (12 bytes)
    uint8_t key[32] = 
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
    };

    uint8_t nonce[12] = {0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x4A, 0x00, 0x00, 0x00, 0x00};

    uint32_t counter=1;
    uint32_t state[16], output[16];
    uint8_t stream[64];

    SetupChaCha20State(state, key, nonce, counter);
    ChaCha20Block(state, output);
    StateToBytes(output, stream);

    //Printing the keystream
    cout<<"ChaCha20 Keystream (64 bytes):"<<endl;
    PrintStream(stream);

    cout<<endl;

    // Code for simulating a differential cryptanalysis attack
    uint8_t nonce1[12] = {0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x4A, 0x00, 0x00, 0x00, 0x00};
    uint8_t nonce2[12] = {0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x4A, 0x00, 0x00, 0x00, 0x00}; // Slightly different
    counter=1;
    // Run differential attack simulation
    SimulateAttack(key, nonce1, nonce2, counter);

    return 0;
}
