import pandas as pd
import matplotlib.pyplot as plt

# reading data from CSV files
s_lock_df = pd.read_csv('comparison/s_lock.csv', delim_whitespace=True)
rw_lock_df = pd.read_csv('comparison/rw_lock.csv', delim_whitespace=True)
hoh_lock_df = pd.read_csv('comparison/hoh_lock.csv', delim_whitespace=True)

def plot_data(workload_type):
    '''
        Function to plot the data read from CSV files

            Parameters:
                workload_type(str): the type of workload to plot
    '''
    data = [s_lock_df['thread_count'], s_lock_df[workload_type], rw_lock_df[workload_type], hoh_lock_df[workload_type]]
    headers = ['no_of_threads', 's_lock', 'rw_lock', 'hoh_lock']
    df = pd.concat(data, axis=1, keys=headers)

    df.plot(x ='no_of_threads', kind = 'line')
    plt.title('Time taken vs No. of Threads - ' + workload_type + ' workload')
    plt.xlabel('No. of Threads')
    plt.ylabel('Time taken(microseconds)')
    plt.savefig('comparison/' + workload_type + '.png')
    plt.show()

# plotting data
plot_data('write_intensive')
plot_data('read_intensive')
plot_data('read_write')