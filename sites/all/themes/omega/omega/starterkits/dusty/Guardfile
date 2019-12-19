notification :off

group :development do

  # Only run Compass if we have a config.rb file in place.
  if File.exists?("config.rb")
    # Compile on start.
    puts `compass compile --time --quiet`

    # https://github.com/guard/guard-compass
    guard :compass do
      watch(%r{.+\.s[ac]ss$})
    end
  end

  ## Uncomment this if you wish to clear the theme registry every time you
  ## change one of the relevant theme files.
  #guard :shell do
  #  puts 'Monitoring theme files.'
  #
  #  watch(%r{.+\.(php|inc|info)$}) { |m|
  #    puts 'Change detected: ' + m[0]
  #    `drush cache-clear theme-registry`
  #    puts 'Cleared theme registry.'
  #  }
  #end

  # https://github.com/guard/guard-livereload.
  # Ignore *.normalize.scss to prevent flashing content when re-rendering.
  guard :livereload do
    watch(%r{^((?!\.normalize\.).)*\.(css|js)$})
  end

end
