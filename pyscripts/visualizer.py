import pandas as pd
import matplotlib.pyplot as plt

def plotPID(file, pltName="pid_plot"):
    df = pd.read_csv(file, sep=";", header=0, error_bad_lines=False)
    df['datetime'] =  pd.to_datetime(df['datetime'])
    t_0 = df['datetime'][0]
    df['t'] = (df.datetime - t_0).dt.total_seconds()

    plt.subplot(411)
    plt.plot(df.t, df.roll_is, label="roll_is")
    plt.plot(df.t, df.pitch_is, label="pitch_is")
    plt.plot(df.t, df.yaw_is, label="yaw_is")
    plt.ylabel('angle error', fontsize=14)
    plt.legend()
    plt.grid()

    plt.subplot(412)
    plt.plot(df.t, df.err_roll, label="err_roll")
    plt.plot(df.t, df.err_pitch, label="err_pitch")
    plt.plot(df.t, df.err_yawn, label="err_yaw")
    plt.ylabel('angle error', fontsize=14)
    plt.legend()
    plt.grid()

    plt.subplot(413)
    plt.plot(df.t, df.lb, label="lb")
    plt.plot(df.t, df.rb, label="rb")
    plt.ylabel('PID control value', fontsize=14)
    plt.legend()
    plt.grid()

    plt.subplot(414)
    plt.plot(df.t, df.lf, label="lf")
    plt.plot(df.t, df.rf, label="rf")
    plt.legend()
    plt.grid()
    plt.xlabel('time', fontsize=14)
    plt.ylabel('PID control value', fontsize=14)

    plt.savefig(pltName + ".png", dpi=300)
    plt.show()

if __name__ == "__main__":
    plotPID("../logs/pid.csv", "../logs/plots/pid")