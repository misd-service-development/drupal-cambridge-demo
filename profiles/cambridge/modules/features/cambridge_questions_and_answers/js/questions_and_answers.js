jQuery(function ($) {

    $('.campl-questions-question').click(function () {
        $(this).next('.campl-questions-answer').toggleClass('campl-questions-answer-revealed');
        $(window).trigger('resize'); // force column heights to be recalculated
    });

});
