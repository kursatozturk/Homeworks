
$(document).ready(function(){
    $('form#readqr').submit(function(e){
        e.preventDefault();
        var formData = new FormData(($(this)[0]));
        $.post({
            method: 'POST',
            url: 'read/',
            data: formData,
            cache: false,
            contentType: false,
            processData: false,
        }).done(function(d){
            if(d['data']){
                console.log(d['data']);
                $('img#qrimg').css('visibility', 'hidden');
                $('span#dataSpan')[0].textContent = d['data'];
                $('#dataHeader').css('visibility', 'visible');
            }
        });
    });

    $('form#createqr').submit(function(e){
        e.preventDefault();
        var formData = new FormData(($(this)[0]));
        $.post({
            url: 'create/',
            data: formData,
            cache: false,
            contentType: false,
            processData: false
        }).done(function(d){    
            $('#qrimg').attr('src', `${d}`);
            $('#qrimg').css('visibility', 'visible');
            $('#dataHeader').css('visibility', 'hidden');
            
        });
    });

});
