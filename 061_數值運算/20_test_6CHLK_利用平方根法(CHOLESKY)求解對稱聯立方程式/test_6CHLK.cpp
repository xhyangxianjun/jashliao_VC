/*
利用平方根法(CHOLESKY)求解對稱聯立方程式
*/
  #include <math.h>
  #include <stdio.h>
  int chlk(double *a,int n,int m,double *d)
  //int n,m;
  //double a[],d[];
  { int i,j,k,u,v;
    if ((a[0]+1.0==1.0)||(a[0]<0.0))
      { printf("fail\n"); return(-2);}
    a[0]=sqrt(a[0]);
    for (j=1; j<=n-1; j++) a[j]=a[j]/a[0];
    for (i=1; i<=n-1; i++)
      { u=i*n+i;
        for (j=1; j<=i; j++)
          { v=(j-1)*n+i;
            a[u]=a[u]-a[v]*a[v];
          }
        if ((a[u]+1.0==1.0)||(a[u]<0.0))
          { printf("fail\n"); return(-2);}
        a[u]=sqrt(a[u]);
        if (i!=(n-1))
          { for (j=i+1; j<=n-1; j++)
              { v=i*n+j;
                for (k=1; k<=i; k++)
                  a[v]=a[v]-a[(k-1)*n+i]*a[(k-1)*n+j];
                a[v]=a[v]/a[u];
              }
          }
      }
    for (j=0; j<=m-1; j++)
      { d[j]=d[j]/a[0];
        for (i=1; i<=n-1; i++)
          { u=i*n+i; v=i*m+j;
            for (k=1; k<=i; k++)
              d[v]=d[v]-a[(k-1)*n+i]*d[(k-1)*m+j];
            d[v]=d[v]/a[u];
          }
      }
    for (j=0; j<=m-1; j++)
      { u=(n-1)*m+j;
        d[u]=d[u]/a[n*n-1];
        for (k=n-1; k>=1; k--)
          { u=(k-1)*m+j;
            for (i=k; i<=n-1; i++)
              { v=(k-1)*n+i;
                d[u]=d[u]-a[v]*d[i*m+j];
              }
            v=(k-1)*n+k-1;
            d[u]=d[u]/a[v];
          }
      }
    return(2);
  }
  main()
  { int i;
	double *pa,*pd;
    static double a[4][4]={ {5.0,7.0,6.0,5.0},
    {7.0,10.0,8.0,7.0},{6.0,8.0,10.0,9.0},{5.0,7.0,9.0,10.0}};
    static double d[4][2]={ {23.0,92.0},{32.0,128.0},
                            {33.0,132.0},{31.0,124.0}};
    pa=(double *)a;
	pd=(double *)d;
	if (chlk(pa,4,2,pd)>0)
      for (i=0; i<=3; i++)
        printf("x(%d)=%13.7e,   %13.7e\n",i,d[i][0],d[i][1]);
  }
