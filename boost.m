t = [0:0.1:5];
function dx=fboost(t,x)
global v_in R C L T d;
dx=zeros(2,1);
if mypwm(t,T,d)==1
    dx(1) = (v_in/L);
    dx(2) = -(x(2)/R*C);
end
if mypwm(t,T,d)==0 && x(1)>=0
    dx(1) = ((x(2)/L)+(v_in/L));
    dx(2) = ((x(1)/C) - (x(2)/R*C));
    end
if mypwm(t,T,d)==0 && x(1) <0
    dx(1) = 0;
    dx(2) = -(x(2)/R*C);
    end
end
