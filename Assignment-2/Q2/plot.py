import pandas as pd
import matplotlib.pyplot as plt

def plot_data(filename):
    '''
    Function to plot the data from the CSV files created by the C program
    
        Parameters: 
            filename(str): string to decide which file to read data from

        Returns: 
            none
    '''
    df = pd.read_csv('data/' + filename + '.csv', delim_whitespace=True)
    plot_df = df[['cache_size', 'FIFO', 'LRU', 'LRUapprox', 'RANDOM']].copy()
    plot_df.plot(x ='cache_size', kind = 'line')
    plt.title('Hit Rate vs Cache Size(' + filename.capitalize() + ' Workload, Pages = ' + str(df.columns[5]) + ', Size = ' + str(df.columns[6]) + ')')
    plt.xlabel('Cache Size')
    plt.ylabel('Hit Rate')
    plt.savefig('plots/' + filename + '_' + str(df.columns[5]) + '_' + str(df.columns[6]) + '.png')
    plt.show()

# call plot_data() to plot the data from the file
plot_data('random')
plot_data('local')
plot_data('loop')


