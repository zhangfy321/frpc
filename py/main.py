import loguru
import worker


if __name__ == '__main__':
    w = worker.Worker()
    w.run()
