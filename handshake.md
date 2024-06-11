# protocol
A protocol based on KWP2000 standard will be used

# Layout of the protocol
## PC application side:
    | Lenght | ID | DATA | CHECKSUM |

Lenght      [1 Byte]    - number of bytes of the message. Maximum number of bytes 255.
ID          [2 Byte]    - ID of the message. First byte will be 0x1E, and the second byte will 
                        be used for the command
DATA        [x Bytes]   - maximum 255 bytes
CHECKSUM    [1 Bytes]   - sum of all BYTES that include ID and DATA 

## Server side:
   | Lenght | ID | DATA | CHECKSUM |

Lenght      [1 Byte]    - number of bytes of the message. Maximum number of bytes 255.
ID          [2 Byte]    - ID of the message. First byte will be 0x1E + 0x30, and the second byte will 
                        be used for the error reporting
DATA        [x Bytes]   - maximum 255 bytes
CHECKSUM    [1 Bytes]   - sum of all BYTES that include ID and DATA 