from typing import Tuple
from http import HTTPStatus

from flask import Flask

from settings import APP_NAME


APP = Flask(APP_NAME)


@APP.route('/healthcheck')
def healthcheck() -> Tuple[str, int]:
    return 'OK', 200


@APP.route('/translate')
def translate():
    """
    Translate a string representation of some bits to another kind of string representation
    (e.g. hex to base64).
    """
    return 'not implemented', HTTPStatus.NOT_IMPLEMENTED


@APP.route('/bitOperate')
def bit_operate():
    """
    Perform a bitwise operator on the given bits (e.g. XORing two hex strings and returning a hex string).
    """
    return 'not implemented', HTTPStatus.NOT_IMPLEMENTED


@APP.route('/decode')
def decode():
    """
    Decode the given string.
    """
    return 'not implemented', HTTPStatus.NOT_IMPLEMENTED


if __name__ == '__main__':
    APP.run()
