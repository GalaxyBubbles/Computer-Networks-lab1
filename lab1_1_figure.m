%%

%%
wait_time=textread("C:\Users\lenovo\Documents\我的个人文件\数媒\计算机网络\lab1-1\lab1-1\stats.txt","%f");
[m,~]=size(wait_time);
x=1:1:m;
p=zeros(11,1);
index=0;
[max_val,~]=max(wait_time);
[min_val,~]=min(wait_time);
step=(max_val-min_val)/20;
for i=min_val:step:max_val-step
    index=index+1;
    count=0;
    for j=1:1:m
       if wait_time(j,1)<i+step
           count=count+1;
       end
    end
    p(index,1)=count/m;
end
x1=min_val:step:max_val-step
x1
p
figure;
xlabel('wait time');
ylabel('probability');
plot(x1,p)
