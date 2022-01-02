'use strict';

$(document).ready(function () {
    console.info('start');
    const endpoint = '/api'
    const val_max = 4096;
    const controller_map = {
        channel1: {
            red: 'c0',
            green: 'c1',
            blue: 'c2'
        },
        channel2: {
            red: 'c3',
            green: 'c4',
            blue: 'c5'
        },
        channel3: {
            red: 'c6',
            green: 'c7',
            blue: 'c8'
        },
        channel4: {
            red: 'c9',
            green: 'c10',
            blue: 'c11'
        },
        channel_uv: {
            uv: 'c12'
        }
    }

    let controller_data = {};
    let initial = true;


    const $get = function () {
        $.ajax({
            dataType: "json",
            method: 'get',
            url: endpoint,
            success: function (data) {
                controller_data = data;
                if (initial) {
                    initial = false;
                    post_init()
                }
            },
            error: function (data) {
                console.error(data)
            }
        });
    };

    const $post = function () {
        $.ajax({
            dataType: "json",
            method: 'post',
            contentType: "application/json; charset=utf-8",
            data: JSON.stringify(controller_data),
            url: endpoint,
            success: function (data) {
                controller_data = data;
                console.log(controller_data);
            },
            error: function (data) {
                console.error(data)
            }
        });
    };

    const encode_channel = function (val) {
        let hex = Math.round(255*val / val_max).toString(16);
        if (hex.length < 2) {
            hex = "0" + hex;
        }
        return hex;
    }

    const encode_channel_hex = function(val) {
        return Math.round(val_max * val/255);
    }

    const color_encode = function (red, green, blue) {
        return '#' + encode_channel(red) + encode_channel(green) + encode_channel(blue);
    }

    const color_decode = function (input_id, color) {
        const r = parseInt(color.slice(1, 3), 16)
        const g = parseInt(color.slice(3, 5), 16)
        const b = parseInt(color.slice(5, 7), 16)
        controller_data[controller_map[input_id].red] = encode_channel_hex(r)
        controller_data[controller_map[input_id].green] = encode_channel_hex(g)
        controller_data[controller_map[input_id].blue] = encode_channel_hex(b)
    }

    const mk_color_input = function (input_id) {
        let $input = $('#' + input_id);
        let color = color_encode(
            controller_data[controller_map[input_id].red],
            controller_data[controller_map[input_id].green],
            controller_data[controller_map[input_id].blue]
        )

        $input.val(color);
        $input.on('change', () => {
            color_decode(input_id, $input.val());
            $post();
        });
    };

    const mk_uv = function(input_id) {
        let $input = $('#' + input_id);
        let val = controller_data[controller_map["channel_uv"].uv]
        $input.attr("checked",val > val_max/2);
        $input.on('change', () => {
            controller_data[controller_map["channel_uv"].uv] = $input.is( ":checked" ) ? val_max:0;
            $post();
        });
    }

    const post_init = function () {
        mk_color_input('channel1');
        mk_color_input('channel2');
        mk_color_input('channel3');
        mk_color_input('channel4');
        mk_uv('channel_uv');
    };

    $get();
});