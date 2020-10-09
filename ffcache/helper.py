import brotli
import gzip
import zlib
from _ffcache import HttpHeader


def decompress(data: bytes, encoding: str) -> bytes:
    if encoding == 'gzip':
        data = gzip.decompress(data)
    elif encoding == 'br':
        data = brotli.decompress(data)
    elif encoding == 'deflate':
        data = zlib.decompress(data)

    return data


def get_encoding(header: HttpHeader):
    return header.headers.get('content-encoding', '').strip().lower()


def save(entry, path):
    encoding = get_encoding(entry.get_header())
    if encoding:
        with open(path, 'wb') as f:
            f.write(decompress(entry.get_data(), encoding))
    else:
        entry.save(path)
