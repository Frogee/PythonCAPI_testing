def plotStdVectors(x, y):
    import numpy as np
    import matplotlib.pyplot as plt
    print "Printing from Python in plotStdVectors()"
    print x
    print y
    x = np.fromiter(x, dtype = np.float)
    y = np.fromiter(y, dtype = np.float)
    print x
    print y
    plt.plot(x, y)
    plt.show()
    return 0
