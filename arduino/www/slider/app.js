Number.prototype.pad = function (length) {
	return (new Array(length + 1).join('0') + this).slice(-length);
}

Zepto(function ($) {

	var value = function (data, label) {
		switch (label) {

			case 'run':
				return (data.match("Run: ([0-9]+)")[1] === '1');
				break;

			case 'running':
				return (data.match("Running: ([0-9]+)")[1] === '1');
				break;

			case 'distance':
				return parseInt(data.match("Distance: ([0-9]+)")[1]);
				break;

			case 'delay':
				return parseInt(data.match("Delay: ([0-9]+)")[1]);
				break;

			case 'steps':
				return parseInt(data.match("Steps per run: ([0-9]+)")[1]);
				break;

			case 'total-runs':
				return parseInt(data.match("Total runs: ([0-9]+)")[1]);
				break;

			case 'runs-completed':
				return parseInt(data.match("Runs completed: ([0-9]+)")[1]);
				break;

			case 'completed':
				if (value(data, 'runs-completed') === 0) return 0;
				return ((value(data, 'runs-completed') / value(data, 'total-runs')) * 100).toFixed(2);
				break;

			case 'slide-duration':
				return parseInt(data.match("Slide duration: ([0-9]+)")[1]);
				break;

		}
	}

	var sendRequest = function (endpoint, callback) {

		$.ajax({
			type: 'GET',
			username: 'root',
			password: 'arduino',
			url: '/arduino/'+endpoint,
			success: function (data, status) {

				if (status !== 'success') return;

				$('li.start-stop button').data('run', value(data, 'run'))

				if (value(data, 'run')) {
					$('li.save button').attr('disabled', 'disabled');
					$('li.reset button').attr('disabled', 'disabled');
					$('li.start-stop button').html('Stop');
				} else {
					$('li.save button').removeAttr('disabled');
					$('li.reset button').removeAttr('disabled');
					$('li.start-stop button').html('Start');
				}

				if ($('li.distance input').val() === '') {
					$('li.distance input').val(value(data, 'distance'));
				}

				if ($('li.delay input').val() === '') {
					$('li.delay input').val(value(data, 'delay'));
				}

				if ($('li.steps input').val() === '') {
					$('li.steps input').val(value(data, 'steps'));
				}

				$('li.details.completed span.value').html(value(data, 'completed')+'%');
				$('li.details.duration span.value').html((value(data, 'slide-duration') / 60).toFixed(1)+' minutes');

				var time = new Date();
				$('li.details.updated span.value').html(time.getHours().pad(2)+':'+time.getMinutes().pad(2)+':'+time.getSeconds().pad(2));

				if (typeof callback === 'function') callback(data);

			}
		});

	}

	// Update status initially
	sendRequest('status', function (data) {

		$('div.loading').hide();

		// Update status every X seconds
		setInterval(function () {
			sendRequest('status');
		}, 15000);

	});

	$('ul.dashboard li.save button').click(function (event) {

		$('div.loading').show();
		sendRequest('distance/'+$('li.distance input').val(), function (data) {
			sendRequest('delay/'+$('li.delay input').val(), function (data) {
				sendRequest('steps/'+$('li.steps input').val(), function (data) {
					$('div.loading').hide();
				});
			});
		});

	});

	$('ul.dashboard li.reset button').click(function (event) {

		$('div.loading').show();
		sendRequest('reset/1', function (data) {
			$('div.loading').hide();
		});

	});

	$('ul.dashboard li.start-stop button').click(function (event) {

		$('div.loading').show();
		sendRequest('run/'+($('li.start-stop button').data('run') ? 0 : 1), function (data) {
			$('div.loading').hide();
		});

	});

});
