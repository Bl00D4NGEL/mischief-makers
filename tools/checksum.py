import sys
import argparse
import hashlib

def verify_checksum(bin_file: str, sha1sum_file: str) -> bool:
    # Read the SHA1 checksum from the binary file
    with open(bin_file, 'rb') as f:
        sha1 = hashlib.sha1()
        while True:
            data = f.read(65536)
            if not data:
                break
            sha1.update(data)
        bin_checksum = sha1.hexdigest()

    # Read the SHA1 checksum from the file that contains the checksum
    with open(sha1sum_file, 'r') as f:
        file_checksum = f.readline().split()[0]

    # Compare the checksums and return the result
    print(f"source: {bin_checksum}; target: {file_checksum}")
    if bin_checksum == file_checksum:
        return True
    else:
        return False

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("binfile", help="binary file")
    parser.add_argument("sha1file", help="sha1sum file")
    args = parser.parse_args()
    
    result = verify_checksum(args.binfile, args.sha1file)

    if result == True:
        print("OK!")
        sys.exit(0)
    else:
        print("NOT OK!!!")
        sys.exit(1)

