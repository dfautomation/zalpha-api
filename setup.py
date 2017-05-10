# -*- coding: utf-8 -*-

import codecs
import os
import re

from setuptools import setup, find_packages


###################################################################

NAME = 'zalpha-api'
PACKAGES = find_packages(where='python')
META_PATH = os.path.join('python', 'zalpha_api', '__init__.py')
KEYWORDS = ['zalpha', 'dfautomation']
CLASSIFIERS = [
    'Intended Audience :: Developers',
    'Natural Language :: English',
    'License :: OSI Approved :: Apache Software License',
    'Operating System :: OS Independent',
    'Programming Language :: Python',
    'Programming Language :: Python :: 2',
    'Programming Language :: Python :: 2.7',
    'Programming Language :: Python :: 3',
    'Programming Language :: Python :: 3.3',
    'Programming Language :: Python :: 3.4',
    'Programming Language :: Python :: 3.5',
]
INSTALL_REQUIRES = ['pyzmq']

###################################################################

HERE = os.path.abspath(os.path.dirname(__file__))


def read(*parts):
    with codecs.open(os.path.join(HERE, *parts), 'rb', 'utf-8') as f:
        return f.read()


META_FILE = read(META_PATH)


def find_meta(meta):
    """
    Extract __*meta*__ from META_FILE.
    """
    meta_match = re.search(
        r'^__{meta}__ = [\'"]([^\'"]*)[\'"]'.format(meta=meta),
        META_FILE, re.M
    )
    if meta_match:
        return meta_match.group(1)
    raise RuntimeError('Unable to find __{meta}__ string.'.format(meta=meta))


if __name__ == '__main__':
    setup(
        name=NAME,
        description=find_meta('description'),
        license=find_meta('license'),
        url=find_meta('uri'),
        version=find_meta('version'),
        author=find_meta('author'),
        author_email=find_meta('email'),
        keywords=KEYWORDS,
        long_description=read('README.md'),
        packages=PACKAGES,
        package_dir={'': 'python'},
        requires=['pyzmq'],
        classifiers=CLASSIFIERS,
        install_requires=INSTALL_REQUIRES,
    )
