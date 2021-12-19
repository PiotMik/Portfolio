# -*- coding: utf-8 -*-
"""

"""

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import statsmodels.tsa.api as smt
import statsmodels.api as sm
import scipy.stats as scs

plt.style.use('ggplot')


def plot_sampling_without_noise():
    """
    Create a plot that explains data sampling withoun noise
    """
    # Create a figure
    np.random.seed(12345)
    fig, ax = plt.subplots(figsize=(10, 5))

    # Good to have
    
    sigma=0.2
    S=np.zeros([50], dtype=float)
    S[0]=10
    x=range(0,50,1)
    for i in x[:-1]:
        S[i+1]=S[i]+S[i]*sigma*np.sqrt(1/50)*np.random.normal()
        
    ax.plot(x,S,label="Price", color='blue',linewidth=1)
    ind=[3*i for i in range(1,15)]
    
    
    ax.scatter(ind,S[ind], label="Sampled Data",color='black')
    # Mark best bid and ask
    
    ax.vlines(24, min(S)-1, S[24], colors='black', linestyles='dotted')
    ax.vlines(21, min(S)-1, S[21], colors='black', linestyles='dotted')
    ax.annotate(r'$\Delta$', (22, min(S)-1),fontsize=15)

    plt.title("Sampling Without Noise")
    plt.xlabel('time')
    plt.ylabel('Price')
    plt.legend()
    plt.show()





def plot_sampling_with_noise():
    """
    Create a plot that explains data sampling withoun noise
    """
    # Create a figure
    np.random.seed(12345)
    fig, ax = plt.subplots(figsize=(8, 4))

    # Good to have
    
    sigma=0.2
    S=np.zeros([50], dtype=float)
    S[0]=10
    x=range(0,50,1)
    for i in x[:-1]:
        
        S[i+1]=S[i]+S[i]*sigma*np.sqrt(1/50)*np.random.normal()
        
    ax.plot(x,S,label="Price", color='blue', linewidth=1)
    ind=[3*i for i in range(1,15)]
    S_til=np.array([s+np.random.normal(scale=0.3) for s in S])
    
    ax.scatter(ind,S_til[ind], label="Sampled Data",color='black')
    # Mark best bid and ask
    
    ax.vlines(24, min(S)-1, S_til[24], colors='black', linestyles='dotted')
    ax.vlines(21, min(S)-1, S_til[21], colors='black', linestyles='dotted')
    ax.annotate(r'$\Delta$', (22, min(S)-1),fontsize=15)

    plt.title("Sampling With Noise")
    plt.xlabel('time')
    plt.ylabel('Price')
    plt.legend()
    plt.show()
    
def time_series_plot(y, lags=None, figsize=(10, 8), style='bmh'):
    if not isinstance(y, pd.Series):
        y = pd.Series(y)
    with plt.style.context(style):    
        fig = plt.figure(figsize=figsize)
        layout = (3, 2)
        ts_ax = plt.subplot2grid(layout, (0, 0), colspan=2)
        acf_ax = plt.subplot2grid(layout, (1, 0))
        pacf_ax = plt.subplot2grid(layout, (1, 1))
        qq_ax = plt.subplot2grid(layout, (2, 0))
        pp_ax = plt.subplot2grid(layout, (2, 1))
        
        y.plot(ax=ts_ax)
        ts_ax.set_title('Time Series Analysis Plots')
        smt.graphics.plot_acf(y,zero=False, lags=lags, ax=acf_ax, alpha=0.5)
        smt.graphics.plot_pacf(y,zero=False, lags=lags, ax=pacf_ax, alpha=0.5)
        sm.qqplot(y, line='s', ax=qq_ax)
        qq_ax.set_title('QQ Plot')        
        scs.probplot(y, sparams=(y.mean(), y.std()), plot=pp_ax)

        plt.tight_layout()
    return
