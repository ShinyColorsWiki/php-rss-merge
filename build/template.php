<?php
require_once './vendor/autoload.php';

use LightnCandy\LightnCandy;

$phpFilePrefix = "\nnamespace RSSMerger\\template;\n";
$templateDir   = __DIR__ . '/../src/template/';
$templateFiles = preg_grep('~\.(handlebars|mustache)$~', scandir($templateDir));

foreach ($templateFiles as $file) {
    $template = file_get_contents($templateDir . $file);
    $phpStr   = LightnCandy::compile($template);
    $data     = '<?php ' . $phpFilePrefix . $phpStr . '?>';
    file_put_contents($templateDir . strtok($file, '.') . '.php', $data);
}
