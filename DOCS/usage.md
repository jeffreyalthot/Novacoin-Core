# Usage

## Configuration

`novacoin.conf` is optional. If present in the working directory, it can override:

- `data_dir`
- `log_file`

Default data directory:

- Windows: `%APPDATA%\\Novacoin`
- Linux/macOS: `$XDG_DATA_HOME/novacoin` if set, otherwise `$HOME/.novacoin`

Default log file path:

- `<data_dir>/novacoin.log`

## CLI Commands

Run one of the generated binaries, for example `novacoind.exe`:

- `createkey`: generate a new key identifier in the keystore
- `listkeys`: list known key identifiers
- `mine`: mine one prototype block
- `show-utxos`: print current UTXO count
