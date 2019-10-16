<?php
require_once('./vendor/autoload.php');

use LightnCandy\LightnCandy;

$phpFilePrefix = "\nnamespace RSSMerger\\template;\n";
$template_dir = __DIR__ . "/../src/template/";
$template_files = preg_grep('~\.(handlebars|mustache)$~', scandir($template_dir));

foreach ($template_files as $file) {
    $template = file_get_contents($template_dir . $file);
    $phpStr = LightnCandy::compile($template);
    $data = "<?php " . $phpFilePrefix . $phpStr . "?>";
    file_put_contents($template_dir . strtok($file, '.') . ".php", $data);
}
