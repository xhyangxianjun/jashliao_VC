clc
clear

stream0 = fopen('P.txt');%%%���o�i
stream1 = fopen('Q.txt');
stream2 = fopen('R.txt');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
data0 = fscanf( stream0 , '%f' );
data1 = fscanf( stream1 , '%f' );
data2 = fscanf( stream2 , '%f' );
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fclose(stream0);
fclose(stream1);
fclose(stream2);
alpha=0.1;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
[hat0,ci0]=mle('norm',data0,alpha)
%[muhat0,sigmahat0,muci0,sigmaci0]=normfit(data0,alpha)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
[hat1,ci1]=mle('norm',data1,alpha)
%[muhat1,sigmahat1,muci1,sigmaci1]=normfit(data1,alpha)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
[hat2,ci2]=mle('norm',data2,alpha)
%[muhat2,sigmahat2,muci2,sigmaci2]=normfit(data2,alpha)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%