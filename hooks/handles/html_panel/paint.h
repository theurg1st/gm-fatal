//https://s9.gifyu.com/images/SVJCx.png https://s12.gifyu.com/images/SVJC6.png
int __fastcall hooks::handles::html_panel_paint(void* html_panel)
{
    static std::once_flag once;
    std::call_once(once, [&]() 
    {     
        globals::menu_panel = html_panel;

        const char* script = xorstr(R"(
            fetch('https://dev.pavetr.ru/uploads/fatal_news.json', { 
              method: 'GET',
              mode: 'no-cors'
            })
            .then(function(response) { return response.json(); })
            .then(function(json) {
              UpdateNewsList(json, false);
            });
        )");

        const char* background_script = xorstr(R"(
        function UpdateBackground() { 
            $('body').css({ 
                'background-image': scope.InGame ? 'none' : 'url("http://somestuff.tech.xsph.ru/img/kami.gif")', 
                'background-size': scope.InGame ? 'auto' : 'cover', 
                'background-repeat': scope.InGame ? 'repeat' : 'no-repeat' 
            }); 
        } 
        setInterval(UpdateBackground, 1);
        )");

        utilities::run_javascript(html_panel, script);
        //utilities::run_javascript(html_panel, background_script);
    });

    return originals::html_panel_paint(html_panel);
}