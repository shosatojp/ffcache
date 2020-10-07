from ffcache import FirefoxCache
import urllib.parse
import os
import brotli
import gzip
import zlib

# replace with your directory
cache_dir = os.path.expanduser('~/.cache/mozilla/firefox/jw2y7wcy.dev-edition-default/cache2/')
out_dir = 'cache'

if not os.path.exists(out_dir):
    os.mkdir(out_dir)

cache = FirefoxCache(cache_dir)

for entry in cache.records:
    url = entry.key
    # print(url)

    filename = urllib.parse.quote(url, safe='')[:255]
    encoding = entry.get_header().headers.get('content-encoding', '').strip().lower()
    out_path = os.path.join(out_dir, filename)

    try:
        if encoding:
            # decompress with python
            data = entry.get_data()
            if encoding == 'gzip':
                data = gzip.decompress(data)
            elif encoding == 'br':
                data = brotli.decompress(data)
            elif encoding == 'deflate':
                data = zlib.decompress(data)

            with open(out_path, 'wb') as f:
                f.write(data)
        else:
            # faster for binary
            entry.save(out_path)

    except Exception as e:
        print('>>>', e)
