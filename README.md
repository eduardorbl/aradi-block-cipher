# ARADI Block Cipher (C Implementation)

This repository contains a **C implementation** of the ARADI block cipher, developed as part of the course **MC938 — Cryptography and Data Security** at UNICAMP. The implementation includes **bit-level shuffle optimizations** and supports multiple configurations for encryption and decryption.

## 📁 Project Structure

The project is organized into the following directories:

```
├── include/  
│   ├── aradi.h  
│   ├── keyschedule.h  
│   ├── linear_maps.h  
│   ├── sbox.h  
│   ├── tables.h  
│   └── utils.h  
├── src/  
│   ├── aradi.c  
│   ├── keyschedule.c  
│   ├── linear_maps.c  
│   ├── sbox.c  
│   ├── tables.c  
│   └── utils.c  
├── test/  
│   ├── test_aradi.c   
│   ├── aradi_test_from_csv.c   
│   ├── test_utils.c  
│   └── tests_aradi_encrypt-decrypt.c  
├── benchmarks/  
│   ├── bench_aradi.c  
│   ├── bench_results.csv  
│   ├── plot_bench.py  
│   └── bench_plot.png  
```

---

## 🧪 Tests

The project includes a comprehensive suite of tests to validate the correctness of the ARADI cipher implementation. Below is a detailed explanation of each test file:

### 1. **`test_aradi.c`**
This file contains **unit tests** for the ARADI cipher. It tests encryption and decryption using predefined plaintext, keys, and expected ciphertext values. The tests verify:
- Correctness of encryption by comparing the output with the expected ciphertext.
- Correctness of decryption by ensuring the decrypted ciphertext matches the original plaintext.

### 2. **`aradi_test_from_csv.c`**
This test validates the cipher using a **CSV dataset** containing plaintext, keys, and expected ciphertext. It performs the following:
- Encrypts the plaintext using the provided key and compares the result with the expected ciphertext.
- Optionally decrypts the ciphertext and verifies that it matches the original plaintext.

#### Usage:
To run this test, ensure the CSV file (e.g., `aradi_simple_dataset.csv`) is in the same directory as the executable.

### 3. **`tests_aradi_encrypt-decrypt.c`**
This file performs **randomized testing** of the encryption and decryption functions. It generates random plaintext and keys, encrypts the plaintext, and then decrypts the ciphertext to verify correctness. The test runs for a configurable number of iterations (default: 1000).

### 4. **`test_utils.c`**
This file tests utility functions such as:
- Combining and splitting 16-bit and 32-bit integers.
- Rotating bits.
- Validating the correctness of helper functions used in the cipher.

#### Compilation and Execution:
```bash
gcc -Iinclude -o test_utils test_utils.c utils.c
./test_utils
```

---

## 📊 Benchmarks

The project includes a benchmarking suite to evaluate the performance of different ARADI cipher configurations. The benchmarks measure the execution time for encryption using:
- **No Shuffle**: Standard ARADI implementation.
- **Shuffle (1 Block)**: Optimized implementation with bit-level shuffle for a single block.
- **Shuffle (2 Blocks)**: Optimized implementation with bit-level shuffle for two blocks.

### Benchmark Files:

1. **`bench_aradi.c`**
   - Measures the execution time of the encryption functions for varying numbers of trials (e.g., 100, 500, 1000, etc.).
   - Outputs the results to a CSV file (`bench_results.csv`).

2. **`bench_results.csv`**
   - Contains the benchmark results in the following format:
     ```
     Trials,No_Shuffle(s),Shuffle_1Block(s),Shuffle_2Blocks(s)
     100,0.000217,0.000230,0.000176
     500,0.001072,0.001556,0.001490
     ...
     ```

3. **`plot_bench.py`**
   - A Python script that generates a performance comparison plot from the benchmark results.
   - The plot is saved as `bench_plot.png`.

#### Running Benchmarks:
To compile and execute the benchmark suite:
```bash
make benchmarks
```
This will:
1. Compile the benchmark executable.
2. Run the benchmarks and generate `bench_results.csv`.
3. Plot the results using `plot_bench.py`.

---

## 🛠️ How to Use

### Compilation
The project uses a **Makefile** for compilation. To build the main test executable:
```bash
make
```

### Running Tests
To execute the test suite:
```bash
./test_executable
```

### Cleaning Up
To remove compiled files and executables:
```bash
make clean
```

### Generating Test Vectors
To generate test vectors for validation:
```bash
make generate_vectors
```

---

## 🖥️ System Requirements

- **Compiler**: `clang` or `gcc` with support for C99 or later.
- **Python**: Version 3.6+ (for plotting benchmarks).
- **Libraries**: `matplotlib` (install using `pip install matplotlib`).

---

## 📈 Results and Observations

The benchmarks demonstrate the performance improvements achieved by the shuffle optimizations:
- **Shuffle (2 Blocks)** consistently outperforms the other configurations, especially for larger numbers of trials.
- The **No Shuffle** implementation is the slowest but serves as a baseline for comparison.

The performance plot (`bench_plot.png`) provides a visual representation of these results.

---

## 📝 Conclusion

This project provides a robust implementation of the ARADI block cipher, complete with testing and benchmarking capabilities. The inclusion of shuffle optimizations highlights the potential for performance improvements in cryptographic algorithms. The provided tests ensure correctness, while the benchmarks offer insights into the efficiency of different configurations.

For further inquiries or contributions, feel free to open an issue or submit a pull request.
```