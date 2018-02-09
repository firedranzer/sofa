import numpy as np
import random
import crochet

def show2Dshape():
    tmp="\n\n\n"\
        +"import numpy as np\n"\
        +"import random\n"\
        +"import time\n"\
        +"import crochet\n"\
        +"import matplotlib.pyplot as plt\n"\
        +"(n,p)=(500,500)\n"\
        +"x , y = np.meshgrid(np.linspace(-0.010,0.010,n),np.linspace(-0.0045,0.0120,p))\n"\
        +"z = np.zeros((n,p))\n"\
        +"for i in range(n):\n"\
        +"    for j in range(p):\n"\
        +"        z[i][j] = evalField(x[i][j], y[i][j], 0.0005)\n"\
        +"plt.figure()\n"\
        +"levels = [0.0]\n"\
        +"contour = plt.contour(x, y, z, levels,)\n"\
        +"plt.clabel(contour, colors = 'k', fmt = '%2.1f', fontsize=12)\n"\
        +"plt.show(block=False)\n"
#        +"time.sleep(2)\n"\
#        +"plt.close()\n"

    return tmp










