/*
 * Hardware-in-the-Loop Benchmarking: SIMON and SPECK Lightweight Cryptography
 * Block Size: 64-bit (two 32-bit words)
 * Key Size: 128-bit (four 32-bit words)
 * Target Architectures: 8-bit AVR (ATmega328P) & 32-bit Xtensa (ESP32)
 * Optimization: GCC -Os (Optimize for Size)
 */

#include <stdint.h>
#include <string.h>

// Macro untuk operasi Circular Shift (Rotasi Bit)
#define ROR32(x, r) (((x) >> (r)) | ((x) << (32 - (r))))
#define ROL32(x, r) (((x) << (r)) | ((x) >> (32 - (r))))

// Konstanta z3 untuk Key Schedule SIMON 64/128
const uint64_t z3 = 0x7ED0A88D16BE2A6BULL;

// ==============================================================================
// IMPLEMENTASI ALGORITMA SPECK 64/128 (ARX Structure: Addition, Rotation, XOR)
// ==============================================================================

void speck64_128_encrypt(const uint32_t pt[2], uint32_t ct[2], const uint32_t key[4]) {
    uint32_t y = pt[0]; // Right word
    uint32_t x = pt[1]; // Left word
    
    uint32_t b = key[0];
    uint32_t a[3] = {key[1], key[2], key[3]};
    
    for (uint32_t i = 0; i < 27; i++) {
        // Enkripsi (Round Function)
        x = (ROR32(x, 8) + y) ^ b;
        y = ROL32(y, 3) ^ x;
        
        // Key Schedule
        uint32_t t = (ROR32(a[i % 3], 8) + b) ^ i;
        b = ROL32(b, 3) ^ t;
        a[i % 3] = t;
    }
    ct[0] = y;
    ct[1] = x;
}

// ==============================================================================
// IMPLEMENTASI ALGORITMA SIMON 64/128 (Feistel Structure & Bitwise Operations)
// ==============================================================================

void simon64_128_encrypt(const uint32_t pt[2], uint32_t ct[2], const uint32_t key[4]) {
    uint32_t y = pt[0]; // Right word
    uint32_t x = pt[1]; // Left word
    uint32_t k[44];     // Key schedule array untuk 44 rounds
    
    // Copy inisial key
    k[0] = key[0]; k[1] = key[1]; k[2] = key[2]; k[3] = key[3];
    
    // Key Expansion (Key Schedule)
    for (int i = 4; i < 44; i++) {
        uint32_t tmp = ROR32(k[i-1], 3);
        tmp = tmp ^ k[i-3];
        tmp = tmp ^ ROR32(tmp, 1);
        k[i] = k[i-4] ^ tmp ^ 0xfffffffc ^ ((z3 >> ((i - 4) % 62)) & 1);
    }
    
    // Enkripsi (Round Function)
    for (int i = 0; i < 44; i++) {
        uint32_t tmp = x;
        x = y ^ (ROL32(x, 1) & ROL32(x, 8)) ^ ROL32(x, 2) ^ k[i];
        y = tmp;
    }
    
    ct[0] = y;
    ct[1] = x;
}

// ==============================================================================
// FUNGSI UTAMA (SETUP & LOOP UNTUK PENGUJIAN HARDWARE)
// ==============================================================================

void setup() {
    Serial.begin(115200);
    
    // Inisialisasi Data Dummy (Payload 32 bytes)
    uint32_t plaintext[2] = {0x656b696c, 0x20646e75}; 
    uint32_t ciphertext[2] = {0};
    uint32_t key[4] = {0x19181110, 0x09080100, 0x1a1b1c1d, 0x0a0b0c0d};

    Serial.println("Memulai Komparasi SIMON dan SPECK...");
    
    // Skenario Pengukuran (Contoh 1 siklus)
    unsigned long startTime = micros();
    speck64_128_encrypt(plaintext, ciphertext, key);
    unsigned long endTime = micros();
    
    Serial.print("Waktu Eksekusi SPECK (us): ");
    Serial.println(endTime - startTime);
}

void loop() {
    // Kosong untuk bare-metal benchmarking
}