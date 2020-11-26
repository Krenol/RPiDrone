import pandas as pd
import matplotlib.pyplot as plt

def plotPID(file, pltName="pid_plot"):
    df = pd.read_csv(file, sep=";", header=0, error_bad_lines=False)
    df['datetime'] =  pd.to_datetime(df['datetime'])
    t_0 = df['datetime'][0]
    df['t'] = (df.datetime - t_0).dt.total_seconds()

    plt.subplot(311)
    plt.plot(df.t, df.err_b, label="err_b")
    plt.plot(df.t, df.err_g, label="err_g")
    plt.ylabel('angle error', fontsize=14)
    plt.legend()

    plt.subplot(312)
    plt.plot(df.t, df.lb, label="lb")
    plt.plot(df.t, df.rb, label="rb")
    plt.ylabel('PID control value', fontsize=14)
    plt.legend()

    plt.subplot(313)
    plt.plot(df.t, df.lf, label="lf")
    plt.plot(df.t, df.rf, label="rf")
    plt.legend()
    plt.xlabel('time', fontsize=14)
    plt.ylabel('PID control value', fontsize=14)

    plt.savefig(pltName + ".png", dpi=300)
    plt.show()
