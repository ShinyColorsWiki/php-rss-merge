<?php

declare(strict_types=1);

namespace RSSMerger;

use Error;
use function header;
use function http_response_code;

/**
 * Class BaseError
 */
class BaseError
{
    /**
     * Error type.
     *
     * @var Error|null error
     */
    public $error;


    /**
     * Constructor
     *
     * @param Error|null $error Error.
     */
    public function __construct(?Error $error = null)
    {
        $this->error = $error;
        $this->logError();
    }


    /**
     * Render Error page to HTML
     *
     * @param int         $code    HTTPCode.
     * @param string|null $message Custom message.
     */
    public function renderHTML(int $code = 500, ?string $message = null) : void
    {
        http_response_code($code);
        $renderer = include 'template/error.php';
        echo $renderer(
            [
                'code'    => $code,
                'message' => $this->readMessage($message),
            ]
        );
    }


    /**
     * Render Error page to XML
     *
     * @param int         $code    HTTPCode.
     * @param string|null $message Custom message.
     */
    public function renderXML(int $code = 500, ?string $message = null) : void
    {
        http_response_code($code);
        header('Content-Type: application/xml; charset=UTF-8');
        $renderer = include 'template/xmlerror.php';
        echo $renderer(
            [
                'code'    => $code,
                'message' => $this->readMessage($message),
            ]
        );
    }


    /**
     * Read message from various source.
     *
     * @param string|null $message A custom message.
     *
     * @return string
     */
    private function readMessage(?string $message) : string
    {
        if ($message !== null) {
            return $message;
        }

        if ($this->error !== null) {
             return $this->error->getMessage();
        }

        return 'Something went wrong';
    }


    /**
     * Log error to logger.
     */
    private function logError() : void
    {
        $GLOBALS['log']->error($this->error);
    }
}
