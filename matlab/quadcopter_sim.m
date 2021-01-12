I_hover = 1200;
I_min = 0;
I_max = 2000;

g = 9.81;
m = 1.2;

% motor dimens
m_m = 0.047;
r_m = 0.028;
h_m = 0.03;
% drone dimens
l = 0.24;
m_d = m - m_m * 4;
alpha = deg2rad(45);
width_d = 0.14;
length_d = 0.2;
height_d = 0.1;
r_d = mean([width_d, length_d, height_d]);

% Inertias
Jx_d = 2 / 3 * m_d * r_d.^2;  
Jy_d = 2 / 3 * m_d * r_d.^2;  
Jz_d = 2 / 3 * m_d * r_d.^2;  
Jx_m = 1 / 12 * m_m * (3 * r_m.^2 + h_m^.2);  
Jy_m = 1 / 12 * m_m * (3 * r_m.^2 + h_m^.2);   
Jz_m = 0.5 * m_m * r_m.^2; 

Jx = Jx_d + 4 * (Jx_m + (sin(alpha) * l).^2 * m_m);
Jy = Jy_d + 4 * (Jy_m + (cos(alpha) * l).^2 * m_m);
Jz = Jz_d + 4 * (Jz_m + m_m * r_d.^2);

Jp = 0.044;     % Moment of Intertia of the rotor (kg-m^2)

% motor stuff
kV = 2400;
max_power = 240; % Watt
max_amp = 13; % Ampere
max_volt = max_power / max_amp; % Volt
motor_eff = 0.8;
motor_gain = m * g * I_hover / I_max / 4 * motor_eff;
I_to_rad_s = convangvel(max_volt * motor_eff * kV / I_max, 'rpm', 'rad/s');
Kt = m * g / 4 / (I_to_rad_s * I_hover).^2; % T/w^2 at hover point

% drone stuff
Kd = 1e-6;


