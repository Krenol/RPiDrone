import pandas as pd
import matplotlib.pyplot as plt

def plotPID(file, pltName="pid_plot"):
    df = pd.read_csv(file, sep=";", header=0, error_bad_lines=False)
    df['datetime'] =  pd.to_datetime(df['datetime'])

    plt.subplot(311)
    plt.plot(df.datetime, df.err_b, label="err_b")
    plt.plot(df.datetime, df.err_g, label="err_g")
    plt.ylabel('angle error', fontsize=14)
    plt.legend()

    plt.subplot(312)
    plt.plot(df.datetime, df.lb, label="lb")
    plt.plot(df.datetime, df.rb, label="rb")
    plt.ylabel('PID control value', fontsize=14)
    plt.legend()

    plt.subplot(313)
    plt.plot(df.datetime, df.lf, label="lf")
    plt.plot(df.datetime, df.rf, label="rf")
    plt.legend()
    plt.xlabel('time', fontsize=14)
    plt.ylabel('PID control value', fontsize=14)

    plt.savefig(pltName + ".png", dpi=300)
    plt.show()
