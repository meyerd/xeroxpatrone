import sys
import feedparser
import urllib
import urlparse

def download_from_atom(feed_url):
  d = feedparser.parse(feed_url)
  links = [l.href for e in d.entries for l in e.links if l.rel == 'direct']
  for link in links:
    print("Downloading %s" % link)
    filename = urlparse.urlsplit(link).path.split("/")[-1]
    urllib.urlretrieve(link, filename)

def main(args):
  if len(args) != 2:
    print("download_from_atom.py <atom feed>")
    return  
  download_from_atom(args[1])

if __name__ == '__main__':
  main(sys.argv)
