d = 0.5;
T = 0.5;
t = [0 100];
fplot(@(t) pwm(t, T, d), [0 100])

function v = mpwm(t, T, d)
    if (0<=mod(t,T)&&mod(t,T)<(d*T))
        v = 1;
    end
    if ((d*T)<=mod(t,T)<(T))
        v = 0;
    end 
end


