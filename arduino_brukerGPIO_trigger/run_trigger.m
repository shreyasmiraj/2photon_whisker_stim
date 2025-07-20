function [] = run_trigger(type, duration, offset)
    %%Execute trigger of predifiend types
    
    switch type
        case {'move_fine', 'move_coarse'}
            if nargin < 2
                error('Move requires offset value to move with');
            elseif nargin == 2
                offset = duration;
            end
        otherwise
            if nargin < 2
                duration = 10;  %sec
                offset = 0;
            end
    end
    arduino_delay = 1.5000;  %sec
    bruker_delay = 0;   %sec
    sample_rate = 19200; %Hz
    trigger_pulse_width = 0.001; %sec
    reset_pulse_width = 0.002;  %sec
    pos_fine_pulse_width = 0.003; %sec
    neg_fine_pulse_width = 0.004; %sec
    pos_coarse_pulse_width = 0.005; %sec
    neg_coarse_pulse_width = 0.006; %sec
    reset_duration = 2.5; %sec
    fine_duration = 0.4; %sec
    coarse_duration = 1; %sec
    switch type
        case {'arduino', 'stimulus_test'}
            arduino_brukerGPIO_trigger('arduino', duration, trigger_pulse_width, arduino_delay, bruker_delay, sample_rate);
        case {'bruker_GPIO', 'control', 'imaging_control'}
            arduino_brukerGPIO_trigger('bruker_GPIO', duration, trigger_pulse_width, arduino_delay, bruker_delay, sample_rate);
        case {'both', 'stimulus', 'imaging_stimulus'}
            arduino_brukerGPIO_trigger('both', duration, trigger_pulse_width, arduino_delay, bruker_delay, sample_rate);
        case {'stimulus_periodic', 'imaging_stimulus_periodic'}
            arduino_brukerGPIO_trigger('imaging_stimulus_periodic', duration, trigger_pulse_width, arduino_delay, bruker_delay, sample_rate);
        case {'reset'}
            arduino_brukerGPIO_trigger('reset', reset_duration, reset_pulse_width, 0, bruker_delay, sample_rate);
        case {'move_fine'}
            if offset > 0
                for i = 1:cast(offset,'int8');
                    arduino_brukerGPIO_trigger('offset', fine_duration, pos_fine_pulse_width, 0, bruker_delay, sample_rate);
                end
            elseif offset < 0
                offset = offset * -1;
                for i = 1:cast(offset, 'int8')
                    arduino_brukerGPIO_trigger('offset', fine_duration, neg_fine_pulse_width, 0, bruker_delay, sample_rate);
                end
            end
        case {'move_coarse'}
            if offset > 0
                for i = 1:cast(offset,'int8');
                    arduino_brukerGPIO_trigger('offset', coarse_duration, pos_coarse_pulse_width, 0, bruker_delay, sample_rate);
                end
            elseif offset < 0
                offset = offset * -1;
                for i = 1:cast(offset, 'int8')
                    arduino_brukerGPIO_trigger('offset', coarse_duration, neg_coarse_pulse_width, 0, bruker_delay, sample_rate);
                end
            end
    end
end

