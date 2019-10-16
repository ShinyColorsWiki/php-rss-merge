<?php

namespace RSSMerger;

use Monolog\Logger;
use Monolog\Handler\ErrorLogHandler;

/**
 * Class Initializer
 */
class Initializer
{


    /**
     * Initialize function.
     *
     * @return void
     */
    public static function initialize() : void
    {
        self::logSetup();

    }//end initialize()


    /**
     * Logger Setup
     *
     * @return void;
     */
    private static function logSetup() : void
    {
        $logger = new Logger('rss-merger');

        $errorLogHandler = new ErrorLogHandler();
        $logger->pushHandler($errorLogHandler);

        $GLOBALS['log'] = $logger;

    }//end logSetup()


}//end class
