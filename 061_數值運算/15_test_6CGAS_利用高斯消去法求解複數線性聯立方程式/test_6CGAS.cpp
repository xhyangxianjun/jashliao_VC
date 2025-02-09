
  #include <stdlib.h>
  #include <stdio.h>
/*
利用高斯消去法求解複數線性聯立方程式
*/
  int cgas(double *ar,double *ai,int n,double *br,double *bi)
  //int n;
  //double ar[],ai[],br[],bi[];
  { int *js,l,k,i,j,is,u,v;
    double p,q,s,d;
    js=(int *)malloc(n*sizeof(int));
    for (k=0;k<=n-2;k++)
      { d=0.0;
        for (i=k;i<=n-1;i++)
        for (j=k;j<=n-1;j++)
          { u=i*n+j;
            p=ar[u]*ar[u]+ai[u]*ai[u];
            if (p>d) {d=p;js[k]=j;is=i;}
          }
        if (d+1.0==1.0)
          { free(js); printf("err**fail\n");
            return(0);
          }
        if (is!=k)
          { for (j=k;j<=n-1;j++)
              { u=k*n+j; v=is*n+j;
                p=ar[u]; ar[u]=ar[v]; ar[v]=p;
                p=ai[u]; ai[u]=ai[v]; ai[v]=p;
              }
            p=br[k]; br[k]=br[is]; br[is]=p;
            p=bi[k]; bi[k]=bi[is]; bi[is]=p;
          }
        if (js[k]!=k)
          for (i=0;i<=n-1;i++)
            { u=i*n+k; v=i*n+js[k];
              p=ar[u]; ar[u]=ar[v]; ar[v]=p;
              p=ai[u]; ai[u]=ai[v]; ai[v]=p;
            }
        v=k*n+k;
        for (j=k+1;j<=n-1;j++)
          { u=k*n+j;
            p=ar[u]*ar[v]; q=-ai[u]*ai[v];
            s=(ar[v]-ai[v])*(ar[u]+ai[u]);
            ar[u]=(p-q)/d; ai[u]=(s-p-q)/d;
          }
        p=br[k]*ar[v]; q=-bi[k]*ai[v];
        s=(ar[v]-ai[v])*(br[k]+bi[k]);
        br[k]=(p-q)/d; bi[k]=(s-p-q)/d;
        for (i=k+1;i<=n-1;i++)
          { u=i*n+k;
            for (j=k+1;j<=n-1;j++)
              { v=k*n+j; l=i*n+j;
                p=ar[u]*ar[v]; q=ai[u]*ai[v];
                s=(ar[u]+ai[u])*(ar[v]+ai[v]);
                ar[l]=ar[l]-p+q;
                ai[l]=ai[l]-s+p+q;
              }
            p=ar[u]*br[k]; q=ai[u]*bi[k];
            s=(ar[u]+ai[u])*(br[k]+bi[k]);
            br[i]=br[i]-p+q; bi[i]=bi[i]-s+p+q;
          }
      }
    u=(n-1)*n+n-1;
    d=ar[u]*ar[u]+ai[u]*ai[u];
    if (d+1.0==1.0)
      { free(js); printf("err**fail\n");
        return(0);
      }
    p=ar[u]*br[n-1]; q=-ai[u]*bi[n-1];
    s=(ar[u]-ai[u])*(br[n-1]+bi[n-1]);
    br[n-1]=(p-q)/d; bi[n-1]=(s-p-q)/d;
    for (i=n-2;i>=0;i--)
    for (j=i+1;j<=n-1;j++)
      { u=i*n+j;
        p=ar[u]*br[j]; q=ai[u]*bi[j];
        s=(ar[u]+ai[u])*(br[j]+bi[j]);
        br[i]=br[i]-p+q;
        bi[i]=bi[i]-s+p+q;
      }
    js[n-1]=n-1;
    for (k=n-1;k>=0;k--)
      if (js[k]!=k)
        { p=br[k]; br[k]=br[js[k]]; br[js[k]]=p;
          p=bi[k]; bi[k]=bi[js[k]]; bi[js[k]]=p;
        }
    free(js);
    return(1);
  }

  void main(void)
  { int i;
    static double ar[4][4]={ {1.0,3.0,2.0,13.0},
                             {7.0,2.0,1.0,-2.0},
                             {9.0,15.0,3.0,-2.0},
                             {-2.0,-2.0,11.0,5.0}};
    static double ai[4][4]={ {3.0,-2.0,1.0,6.0},
                             {-2.0,7.0,5.0,8.0},
                             {9.0,-3.0,15.0,1.0},
                             {-2.0,-2.0,7.0,6.0}};
    static double br[4]={2.0,7.0,3.0,9.0};
    static double bi[4]={1.0,2.0,-2.0,3.0};
	double *par,*pai,*pbr,*pbi;
	par=(double *)ar;
	pai=(double *)ai;
	pbr=(double *)br;
	pbi=(double *)bi;
    if (cgas(par,pai,4,pbr,pbi)!=0)
      for (i=0;i<=3;i++)
        printf("b(%d)=%13.7e  +j %13.7e\n",i,br[i],bi[i]);
  }