from typing import Tuple

from flask import Flask

from settings import APP_NAME


APP = Flask(APP_NAME)


@APP.route('/healthcheck')
def healthcheck() -> Tuple[str, int]:
    return 'OK', 200


@APP.route('/hello')
def say_hello():
    return 'Hello', 200


if __name__ == '__main__':
    APP.run()
