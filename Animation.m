cart_width = .5;
cart_height = .2;
pole_length = .5;

fps = 50; % Hz

figure('color','white');

[generation, fitness, k, l, m, n, time_step, fail_position] = textread('./build/Debug/header.dat', '%d %f %f %f %f %f %f %f', 'headerlines', 1);

title({['Generation: ', num2str(generation)], ['Fitness: ', num2str(fitness)]});

% hide Y axis
set(gca, 'YTickLabelMode', 'Manual')
set(gca, 'YTick', [])
set(gca, 'YColor', [1 1 1])

axis ([(-fail_position - cart_width / 2) (fail_position + cart_width / 2) -2 2]);

[cart_position, pole_angle] = textread('./build/Debug/movement.dat', '%f %f', 'headerlines', 1);

Frames = moviein(length(cart_position));

hold on

cart = rectangle();
pole = line([0 0], [1 1], 'color', [.4 .7 .3], 'lineWidth', 2);

for j = 1:length(cart_position)
    set(cart,'Position', [(cart_position(j) - cart_width / 2), 0, cart_width, cart_height]);
    set(pole,'XData', [cart_position(j), (cart_position(j) + cos(pole_angle(j)))]);
    set(pole,'YData', [(cart_height / 2), ((cart_height / 2) + sin(pole_angle(j)))]);
    refreshdata;
    drawnow;
    Frames(:, j) = getframe;
end

movie(Frames, 1, fps);
%mpgwrite(Frames,colormap,'render.mpg');
