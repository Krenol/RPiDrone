import pandas as pd
import matplotlib.pyplot as plt
from scipy.signal import lfilter
import re

def getNetworkTimes(cons, t_0):
    ret = []
    for l in cons:
        spl = l.split(' ')
        t = pd.to_datetime(spl[0] + " " + spl[1])
        ret.append((t - t_0).total_seconds())
    return ret
    

def plotPID(pid_file, power_file, exec_file, network_log, pltName="plot"):
    df = pd.read_csv(pid_file, sep=";", header=0, error_bad_lines=False)
    df['datetime'] =  pd.to_datetime(df['datetime'])
    t_0 = df['datetime'][0]
    df['t'] = (df.datetime - t_0).dt.total_seconds()
    textfile = open(network_log, 'r')
    filetext = textfile.read()
    textfile.close()
    conStrs = re.findall(".*(?:INFO connected to)", filetext)
    disconStrs = re.findall(".*(?:ERROR lost connection to client)", filetext)

    cons = getNetworkTimes(conStrs, t_0)
    discons = getNetworkTimes(disconStrs, t_0)

    plt.subplot(5,2,1)
    plt.plot(df.t, df.vel_x_is, label="x_is")
    plt.plot(df.t, df.vel_y_is, label="y_is")
    plt.plot(df.t, df.vel_z_is, label="z_is")
    min_val = min(df.vel_x_is.min(), df.vel_y_is.min(), df.vel_z_is.min(), -0.1) * 1.1
    max_val = max(df.vel_x_is.max(), df.vel_y_is.max(), df.vel_z_is.max(), 0.1) * 1.1
    plt.vlines(cons, min_val, max_val, 'g', linestyles='dashed')
    plt.vlines(discons, min_val, max_val, 'r', linestyles='dashed')
    plt.ylabel('is vel')
    plt.legend()
    plt.grid()

    plt.subplot(5,2,2)
    plt.plot(df.t, df.err_vel_x, label="err_x")
    plt.plot(df.t, df.err_vel_y, label="err_y")
    plt.plot(df.t, df.err_vel_z, label="err_z")
    min_val = min(df.err_vel_x.min(), df.err_vel_y.min(), df.err_vel_z.min(), -0.1) * 1.1
    max_val = max(df.err_vel_x.max(), df.err_vel_y.max(), df.err_vel_z.max(), 0.1) * 1.1
    plt.vlines(cons, min_val, max_val, 'g', linestyles='dashed')
    plt.vlines(discons, min_val, max_val, 'r', linestyles='dashed')
    plt.ylabel('vel error')
    plt.legend()
    plt.grid()

    plt.subplot(5,2,3)
    plt.plot(df.t, df.roll_is, label="roll_is")
    plt.plot(df.t, df.pitch_is, label="pitch_is")
    min_val = min(df.roll_is.min(), df.pitch_is.min()) * 1.1
    max_val = max(df.roll_is.max(), df.pitch_is.max()) * 1.1
    plt.vlines(cons, min_val, max_val, 'g', linestyles='dashed')
    plt.vlines(discons, min_val, max_val, 'r', linestyles='dashed')
    plt.ylabel('is angles')
    plt.legend()
    plt.grid()

    plt.subplot(5,2,4)
    plt.plot(df.t, df.err_roll, label="err_roll")
    plt.plot(df.t, df.err_pitch, label="err_pitch")
    min_val = min(df.err_roll.min(), df.err_pitch.min()) * 1.1
    max_val = max(df.err_roll.max(), df.err_pitch.max()) * 1.1
    plt.vlines(cons, min_val, max_val, 'g', linestyles='dashed')
    plt.vlines(discons, min_val, max_val, 'r', linestyles='dashed')
    plt.ylabel('angle error')
    plt.legend()
    plt.grid()

    plt.subplot(5,2,5)
    plt.plot(df.t, df.lb, label="lb")
    plt.plot(df.t, df.rb, label="rb")
    min_val = min(df.lb.min(), df.rb.min()) * 1.1
    max_val = max(df.lb.max(), df.rb.max()) * 1.1
    plt.vlines(cons, min_val, max_val, 'g', linestyles='dashed')
    plt.vlines(discons, min_val, max_val, 'r', linestyles='dashed')
    plt.ylabel('PID control value')
    plt.legend()
    plt.grid()

    plt.subplot(5,2,6)
    plt.plot(df.t, df.lf, label="lf")
    plt.plot(df.t, df.rf, label="rf")
    min_val = min(df.lf.min(), df.rf.min()) * 1.1
    max_val = max(df.lf.max(), df.rf.max()) * 1.1
    plt.vlines(cons, min_val, max_val, 'g', linestyles='dashed')
    plt.vlines(discons, min_val, max_val, 'r', linestyles='dashed')
    plt.ylabel('PID control value')
    plt.legend()
    plt.grid()

    plt.subplot(5,2,7)
    plt.plot(df.t, df.throttle, label="throttle")
    min_val = df.throttle.min() * 1.1
    max_val = df.throttle.max() * 1.1
    plt.vlines(cons, min_val, max_val, 'g', linestyles='dashed')
    plt.vlines(discons, min_val, max_val, 'r', linestyles='dashed')
    plt.legend()
    plt.grid()
    plt.ylabel('throttle')

    df = pd.read_csv(exec_file, sep=";", header=0, error_bad_lines=False)
    df['datetime'] =  pd.to_datetime(df['datetime'])
    t_0 = df['datetime'][0]
    df['t'] = (df.datetime - t_0).dt.total_seconds()
    cons = getNetworkTimes(conStrs, t_0)
    discons = getNetworkTimes(disconStrs, t_0)


    plt.subplot(5,2,8)
    plt.plot(df.t, df.t_exec, label="exec time")
    x = [df.t[0], df.t[df.last_valid_index()]]
    y = df.t_exec.median()
    plt.plot(x, [y, y], label="median exec time")
    y = df.t_exec.mean()
    plt.plot(x, [y, y], label="mean exec time")
    min_val = df.t_exec.min() * 1.1
    max_val = df.t_exec.max() * 1.1
    plt.vlines(cons, min_val, max_val, 'g', linestyles='dashed')
    plt.vlines(discons, min_val, max_val, 'r', linestyles='dashed')
    plt.legend()
    plt.grid()
    plt.ylabel('throttle per esc')


    df = pd.read_csv(power_file, sep=";", header=0, error_bad_lines=False)
    df['datetime'] =  pd.to_datetime(df['datetime'])
    t_0 = df['datetime'][0]
    df['t'] = (df.datetime - t_0).dt.total_seconds()
    cons = getNetworkTimes(conStrs, t_0)
    discons = getNetworkTimes(disconStrs, t_0)


    n = 5  # the larger n is, the smoother curve will be
    b = [1.0 / n] * n
    a = 1
    yy = lfilter(b,a,df.proc_cpu)

    plt.subplot(5,2,9)
    plt.plot(df.t, df.sys_cpu, label="sys_cpu")
    plt.plot(df.t, df.proc_cpu, label="proc_cpu")
    plt.plot(df.t, yy, label="proc_cpu_lfilter")
    plt.ylabel('CPU consumption [%]')
    min_val = min(df.sys_cpu.min(), df.proc_cpu.min()) * 1.1
    max_val = max(df.sys_cpu.max(), df.proc_cpu.max()) * 1.1
    plt.vlines(cons, min_val, max_val, 'g', linestyles='dashed')
    plt.vlines(discons, min_val, max_val, 'r', linestyles='dashed')
    plt.legend()
    plt.grid()
    plt.xlabel('time')

    plt.subplot(5,2,10)
    plt.plot(df.t, df.memory, label="memory")
    plt.plot(df.t, df.virtual_memory, label="virtual_memory")
    plt.ylabel('Memory consumption [MB]')
    min_val = min(df.virtual_memory.min(), df.memory.min()) * 1.1
    max_val = max(df.virtual_memory.max(), df.memory.max()) * 1.1
    plt.vlines(cons, min_val, max_val, 'g', linestyles='dashed')
    plt.vlines(discons, min_val, max_val, 'r', linestyles='dashed')
    plt.legend()
    plt.grid()
    plt.xlabel('time')

    plt.savefig(pltName + ".png", bbox_inches='tight')
    plt.show()

if __name__ == "__main__":
    plotPID("../logs/pid.csv", "../logs/power.csv", "../logs/exec.csv", "../logs/network.log", "../logs/plots/plot")