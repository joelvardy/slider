Zepto(function ($) {

	var sendRequest = function (endpoint) {

		$.ajax({
			type: 'GET',
			username: 'root',
			password: 'arduino',
			url: '/arduino/'+endpoint
		});

	}

	$('button.l13').click(function (e) {

		if ($(this).hasClass('active')) {

			sendRequest('digital/13/0');

			$(this).html('Inactive');
			$(this).removeClass('active');

		} else {

			sendRequest('digital/13/1');

			$(this).html('Active');
			$(this).addClass('active');

		}

	});

});
