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
- `getblockcount`: print current chain height (block count)
- `gettip`: print current tip block hash (`none` if chain is empty)
- `getbestblockhash`: print only the tip hash (`none` if chain is empty)
- `getblockexists <hash>`: check whether a block file exists on disk
- `signmessage <keyid> <message>`: sign a message using a known key identifier
- `verifymessage <keyid> <message> <signaturehex>`: verify a signature for a key/message pair
- `savekeys`: save the keystore to `<data_dir>/keystore.dat`
- `loadkeys`: load the keystore from `<data_dir>/keystore.dat`
- `help`: show available commands
