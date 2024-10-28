# Stable Primitivity Rank Calculator

Welcome to the **Stable Primitivity Rank Calculator**! This program computes the **stable primitivity rank** (\( \text{s}\pi^{(m)}(w) \)) of words in a free group, an important concept in algebra and group theory. Whether you're a mathematician, a student, or just curious, this tool is designed to be accessible—even if you have no prior experience with coding or command-line interfaces.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Getting Started](#getting-started)
  - [System Requirements](#system-requirements)
  - [Download](#download)
  - [Installation](#installation)
- [Using the Program](#using-the-program)
  - [Graphical User Interface (GUI)](#graphical-user-interface-gui)
  - [Command-Line Interface (CLI)](#command-line-interface-cli)
- [Understanding the Concepts](#understanding-the-concepts)
  - [Free Groups and Words](#free-groups-and-words)
  - [Stable Primitivity Rank](#stable-primitivity-rank)
- [Examples](#examples)
- [Troubleshooting](#troubleshooting)
- [Support and Contributions](#support-and-contributions)
- [License](#license)

---

## Introduction

The **Stable Primitivity Rank Calculator** is a tool that calculates algebraic invariants of words in free groups, specifically the stable primitivity rank (\( \text{s}\pi^{(m)}(w) \)) and its variants. These invariants are significant in various fields of mathematics, including algebra, topology, and combinatorics.

This program allows you to:

- Compute the stable primitivity rank for individual words.
- Run statistical analyses over many words of specified lengths.
- Explore concepts in free group theory without needing to delve into complex computations manually.

---

## Features

- **User-Friendly Interface**: A simple GUI that anyone can navigate.
- **Single Word Calculation**: Compute \( \text{s}\pi^{(m)}(w) \) for a specific word.
- **Statistical Analysis**: Generate random words within a length range and compute statistics.
- **Customizable Parameters**: Adjust the rank of the free group and other parameters.
- **Detailed Output**: Provides comprehensive results, including mean and standard deviation for statistical runs.
- **Cross-Platform Support**: Available for Windows, macOS, and Linux.

---

## Getting Started

### System Requirements

- **Operating System**: Windows 7 or later, macOS 10.12 or later, or a recent Linux distribution.
- **Processor**: 1 GHz or faster.
- **Memory**: 512 MB RAM minimum.
- **Disk Space**: 50 MB free space.
- **Dependencies**: None. All necessary libraries are included in the executable.

### Download

Download the latest version of the **Stable Primitivity Rank Calculator** from our [GitHub Releases page](https://github.com/nlevhari/StablePrimitivityRankCalculator/releases).

### Installation

1. **Windows**:
   - Download the `StablePrimitivityRankCalculator.exe` file.
   - Double-click the file to run the program. No installation is required.

2. **macOS**:
   - Download the `StablePrimitivityRankCalculator.dmg` file.
   - Open the `.dmg` file and drag the application to your `Applications` folder.

3. **Linux**:
   - Download the `StablePrimitivityRankCalculator.AppImage` file.
   - Make the file executable: `chmod +x StablePrimitivityRankCalculator.AppImage`.
   - Run the program: `./StablePrimitivityRankCalculator.AppImage`.

---

## Using the Program

GUI - coming soon.

<!-- ### Graphical User Interface (GUI)

The GUI is designed to be intuitive and straightforward.

#### Steps to Use:

1. **Launch the Program**:
   - Double-click the application icon to open the program.

2. **Select Mode**:
   - Choose between **Single Word Calculation** or **Statistical Analysis**.

3. **Single Word Calculation**:
   - **Problem Type**: Select the problem type (e.g., Stable Primitivity Rank).
   - **Rank (r)**: Enter the rank of the free group (a positive integer).
   - **Parameter (m)**: If required, enter the value of `m` (positive integer).
   - **Enter Word**:
     - Input the word using integers to represent generators and their inverses.
     - **Generators**: Use numbers `1` to `r` for generators \( a_1 \) to \( a_r \).
     - **Inverses**: Use numbers `-1` to `-r` for inverses of \( a_1 \) to \( a_r \).
     - **Example**: To input \( a_1 a_2^{-1} a_3 \), enter `1 -2 3`.

4. **Statistical Analysis**:
   - **Problem Type**: Select the problem type.
   - **Rank (r)**: Enter the rank of the free group.
   - **Parameter (m)**: If required, enter the value of `m`.
   - **Minimum Length**: Enter the minimum length of words to generate.
   - **Maximum Length**: Enter the maximum length.
   - **Samples per Length**: Enter the number of words to generate for each length.
   - Click **Run Analysis**.

5. **View Results**:
   - For single calculations, the stable primitivity rank will be displayed.
   - For statistical analyses, a table or graph showing the mean and standard deviation for each length will appear.

6. **Save Results**:
   - You can save the results to a file for later reference. -->

### Command-Line Interface (CLI)

For advanced users, the program also supports CLI usage.

#### Usage:

```bash
./StablePrimitivityRankCalculator [mode] [arguments]
```

#### Modes and Arguments:

- **Single Word Calculation**:

  ```bash
  ./StablePrimitivityRankCalculator single [problem_type] [r] [m (if required)] [word_as_integers]
  ```

- **Statistical Analysis**:

  ```bash
  ./StablePrimitivityRankCalculator stats [problem_type] [r] [m (if required)] [min_length] [max_length] [samples_per_length]
  ```

#### Example:

```bash
./StablePrimitivityRankCalculator single 1 2 3 1 -2 3
```

---

## Understanding the Concepts

### Free Groups and Words

A **free group** is a group where every element (word) is a combination of generators and their inverses without any relations other than the group axioms. For example, in a free group of rank 2, you have generators \( a_1 \) and \( a_2 \).

### Stable Primitivity Rank

The **stable primitivity rank** (\( \text{s}\pi^{(m)}(w) \)) of a word \( w \) measures how "non-primitive" \( w \) is within the free group, considering coverings and subgroups. It's a refined concept that captures deeper algebraic properties of the word.

---

## Examples

### Single Word Calculation

- **Problem**: Compute \( \text{s}\pi^{(m)}(w) \) for \( w = a_1 a_2^{-1} a_1 \) in a free group of rank 2.
- **Steps**:
  1. Open the program.
  2. Select **Single Word Calculation**.
  3. Set **Problem Type** to `Stable Primitivity Rank`.
  4. Enter **Rank (r)**: `2`.
  5. Enter **Word**: `1 -2 1`.
  6. Click **Calculate**.
- **Result**: The program displays the stable primitivity rank.

### Statistical Analysis

- **Problem**: Analyze words of lengths 5 to 10 in a free group of rank 2.
- **Steps**:
  1. Open the program.
  2. Select **Statistical Analysis**.
  3. Set **Problem Type** to `Stable Primitivity Rank`.
  4. Enter **Rank (r)**: `2`.
  5. Set **Minimum Length**: `5`.
  6. Set **Maximum Length**: `10`.
  7. Set **Samples per Length**: `100`.
  8. Click **Run Analysis**.
- **Result**: The program displays a summary of the mean and standard deviation of \( \text{s}\pi^{(m)}(w) \) for each word length.

---

## Troubleshooting

- **The program doesn't start**:
  - Ensure your system meets the requirements.
  - On macOS, you may need to allow the app to run from **System Preferences > Security & Privacy**.
- **Incorrect results**:
  - Double-check the input format.
  - Ensure you're using the correct problem type and parameters.
- **Program crashes**:
  - Try restarting the program.
  - If the problem persists, contact support.

---

## Support and Contributions

If you encounter any issues, have questions, or want to contribute:

- **Email**: [nlevhari@gmail.com](mailto:nlevhari@gmail.com)
- **GitHub Issues**: [GitHub Repository](https://github.com/nlevhari/stable_invariants/issues)
- **Contributions**: Feel free to fork the repository and submit pull requests.

---

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

Thank you for using the **Stable Primitivity Rank Calculator**! We hope it aids in your exploration of free group theory.