import pandas as pd
import matplotlib.pyplot as plt
from scipy.signal import lfilter

def plotPower(file, pltName="power_plt"):
    df = pd.read_csv(file, sep=";", header=0, error_bad_lines=False)
    df['datetime'] =  pd.to_datetime(df['datetime'])
    t_0 = df['datetime'][0]
    df['t'] = (df.datetime - t_0).dt.total_seconds()

    n = 5  # the larger n is, the smoother curve will be
    b = [1.0 / n] * n
    a = 1
    yy = lfilter(b,a,df.proc_cpu)


    plt.subplot(211)
    plt.plot(df.t, df.sys_cpu, label="sys_cpu")
    plt.plot(df.t, df.proc_cpu, label="proc_cpu")
    plt.plot(df.t, yy, label="proc_cpu_lfilter")
    plt.ylabel('CPU consumption [%]', fontsize=14)
    plt.legend()
    plt.grid()

    plt.subplot(212)
    plt.plot(df.t, df.memory, label="memory")
    plt.plot(df.t, df.virtual_memory, label="virtual_memory")
    plt.ylabel('Memory consumption [MB]', fontsize=14)
    plt.legend()
    plt.grid()

    plt.savefig(pltName + ".png", dpi=300)
    plt.show()

if __name__ == "__main__":
    plotPower("../logs/power.csv", "../logs/plots/power")