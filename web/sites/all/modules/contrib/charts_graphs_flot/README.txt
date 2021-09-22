


-- Dates

Flot has a special way of dealing with dates, that is supported by this module.

In your view, specify the output format of the field to 'Custom' and the value of the formatter that appears to 'U'
This will render the field as amount of seconds since Jan 1, 1970 which this module will pick up and process.

(Timezone correction is not implemented yet, so if hours & minutes is your thing, beware)