<?php
    require './config.php';
    global $feed;


function normal()
{
    global $feed;
    echo '<h1>RSS Merge Service</h1>';
    echo '<h3>Currently serving ' . count($feed) . ' services.';
    echo '<br /><br />';
    echo '<style>table, th, td { border: 1px solid black; }</style>';
    foreach ($feed as $f) {
        $feed_url = (isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] === 'on' ? "https" : "http") .
            "://$_SERVER[HTTP_HOST]" . strtok($_SERVER['REQUEST_URI'], '?') . '?feed=' . $f->id;
        echo '<table>';
        echo '<tr>';
        echo '<th>Key</th>';
        echo '<th>Value</th>';
        echo '</tr>';
        echo '<tr>';
        echo '<td>Title</td>';
        echo '<td>' . $f->title . '</td>';
        echo '</tr>';
        echo '<tr>';
        echo '<td>Link</td>';
        echo '<td>' . $f->link . '</td>';
        echo '</tr>';
        echo '<tr>';
        echo '<td>Description</td>';
        echo '<td>' . $f->description . '</td>';
        echo '</tr>';
        echo '<tr>';
        echo '<td>Time-to-Live</td>';
        echo '<td>' . $f->ttl . '</td>';
        echo '</tr>';
        echo '<tr>';
        echo '<td>Encoding</td>';
        echo '<td>' . $f->encoding . '</td>';
        echo '</tr>';
        echo '<tr>';
        echo '<td>Language</td>';
        echo '<td>' . $f->lang . '</td>';
        echo '</tr>';
        echo '<tr>';
        echo '<td>Feed Sources</td>';
        echo '<td>' . join('<br />', $f->feeds) . '</td>';
        echo '</tr>';
        echo '<tr>';
        echo '<td>Feed Url</td>';
        echo '<td><a href="' . $feed_url . '">' . $feed_url . '</a></td>';
        echo '</tr>';
        echo '</table>';
    }

    $wiki_url = 'https://wiki.shinycolo.rs/';
    echo '<br /> <br />By <a href="' . $wiki_url . '">ShinyWiki</a>';
}

$f = $_GET['feed'];
if (isset($f)) {
    $builder = new Builder($feed);
    if (array_key_exists($f, $builder->lists)) {
        try {
            $output = $builder->generateOutput($f);
            header('Content-Type: application/rss+xml; charset=UTF-8');
            echo $output;
        } catch (Exception $e) {
            http_response_code(500);
            echo $e->getMessage();
            echo '<h1>Something went wrong.</h1>';
            normal();
        }
    } else {
        http_response_code(404);
        echo '<h1>No Feed Found. Check list</h1>';
        normal();
    }
    unset($builder);
} else {
    normal();
}
unset($f);
