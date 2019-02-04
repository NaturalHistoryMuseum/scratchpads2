const Element = require('./element');

class Checkbox extends Element {
  isChecked() {
    return this.client.isElementSelected(this.id);
  }

  async check(isChecked = true) {
    if (!isChecked !== !await this.isChecked()) {
      await this.click();
    }
  }
}

module.exports = Checkbox;
