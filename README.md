# Lapis

Lapis is a simple image pixelator created in C.

## Installation

*Note: Lapis is designed for macOS on Intel processors.*

1. **Install Homebrew** (if not already installed):
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

2. **Install libpng**:
   ```bash
   brew install libpng
   ```

3. **Download the latest binary** from the [releases page](https://github.com/xDefyingGravity/Lapis/releases/).

4. **Add Lapis to your PATH**:
   ```bash
   mv ~/Downloads/lapis /usr/local/bin/lapis && chmod +x /usr/local/bin/lapis
   ```

5. **You can now use Lapis!**

## CLI Usage

*Note: Lapis only supports PNG files.*

```bash
lapis depth input_file.png output_file.png
```

The higher the `depth`, the more pixelated the output will be.

## API Usage

```c
#include "lapis.h"

int main(int argc, char** argv) {
    int depth = 6;
    const char* input = "input.png";
    const char* output = "output.png";
    convert(&depth, input, output);

    return 0;
}
```