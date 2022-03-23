from cs50 import get_int


def main():
    # Ask user for height of pyramid to build
    # accepting 1 through 8 inclusive
    while True:
        height = get_int('Height: ')
        if 0 < height < 9:
            break

    # Row will be used to track which row we
    # are currently building
    row = height

    build_pyramid(height, row)


# Build a Mario pyramid of a given height
def build_pyramid(height, row):
    if row > 1:
        build_pyramid(height, row - 1)

    print(((height - row) * ' ') + ('#' * row) + '  ' + ('#' * row))


main()
