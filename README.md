# SSOED (Symmetric Search Over Encrypted Data)

## Overview

The Symmetric Search Over Encrypted Data (SSOED) project is a personal endeavor aimed at implementing the scheme proposed by Dawn Xiaodong Song, David Wagner, and Adrian Perrig in their [paper](ssoed.pdf). The project focuses on enabling symmetric search operations over encrypted data, offering a practical solution for secure data retrieval and manipulation while preserving privacy.

## Features

- **Symmetric Search**: Allows users to perform search operations over encrypted data without compromising data security.

- **Robust Encryption**: Implements strong encryption algorithms to ensure that sensitive data remains protected against unauthorized access.

- **Secure Decryption**: Provides secure decryption capabilities, ensuring that only authorized users can access decrypted data.

## Installation

1. Install the **Sodium library** by following the instructions provided on the [official website](https://libsodium.gitbook.io/doc/).

2. Set the environment variables `INCLUDE_DIR` and `LIB_DIR` to point to the directory containing the header files and the directory containing the Sodium library, respectively. For example:

    ```bash
    export INCLUDE_DIR=/path/to/libsodium/include
    export LIB_DIR=/path/to/libsodium/lib
    ```

3. Clone the repository from GitHub:

    ```bash
    git clone https://github.com/K0ntr4/Symmetric-Search-Over-Encrypted-Data-C.git
    ```

4. Navigate to the project directory:

    ```bash
    cd ssoed
    ```

5. Compile the program using the provided Makefile:

    ```bash
    make all
    ```

6. Run the compiled executable:

    ```bash
    ./ssoed.out
    ```

<!---
7. Follow the on-screen instructions to perform symmetric search operations over encrypted data. 
-->

## Dependencies

- [Sodium Library](https://libsodium.gitbook.io/doc/): A modern, easy-to-use software library for encryption, decryption, signatures, password hashing, and more.

## Contributing

Contributions to the project are welcome! If you have ideas for new features, improvements, or bug fixes, feel free to submit a pull request. Please adhere to the established coding conventions and follow the project's code of conduct.

Contributors are encouraged to follow the style guides enforced by clang-tidy and clang-format. Makefile actions are provided for both tools, but contributors will need to install both packages and run bear make all to generate the configuration file. The bear package is also required for this setup.

Additionally, please ensure that any new code or changes are in line with the principles outlined in the research paper by Dawn Xiaodong Song, David Wagner, and Adrian Perrig, titled "Practical Techniques for Searches on Encrypted Data", as this project aims to implement their scheme for Symmetric Search Over Encrypted Data (SSOED).

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgements

- Special thanks to the developers of the Sodium library for providing a powerful and reliable cryptographic toolkit.
- This project is based on the scheme proposed by Dawn Xiaodong Song, David Wagner, and Adrian Perrig in their paper titled "Practical Techniques for Searches on Encrypted Data."