from loguru import logger
from baseserver import Server
logger.add("log/web.log")


if __name__ == '__main__':
    s = Server()
    s.Run()
